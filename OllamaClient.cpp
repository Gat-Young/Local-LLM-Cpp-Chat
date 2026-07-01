#include "OllamaClient.h"

#if __has_include(<httplib.h>)
#include <httplib.h>
#elif __has_include("external/http/httplib.h")
#include "external/http/httplib.h"
#else
#error "cpp-httplib header not found. Install cpp-httplib or place httplib.h at external/http/httplib.h."
#endif

#if __has_include(<nlohmann/json.hpp>)
#include <nlohmann/json.hpp>
#elif __has_include("external/json/json.hpp")
#include "external/json/json.hpp"
#else
#error "nlohmann/json header not found. Install nlohmann-json or place json.hpp at external/json/json.hpp."
#endif

#include <string>

namespace
{
AIResponse makeSuccessResponse(const std::string& content)
{
    return AIResponse{true, content, ""};
}

AIResponse makeFailureResponse(const std::string& errorMessage)
{
    return AIResponse{false, "", errorMessage};
}

int normalizeTimeoutSeconds(int timeoutSeconds)
{
    return timeoutSeconds > 0 ? timeoutSeconds : 30;
}

bool containsModelNotFoundText(const std::string& text)
{
    return text.find("model") != std::string::npos && text.find("not found") != std::string::npos;
}
}

OllamaClient::OllamaClient(const AppConfig& config)
    : config(config)
{
}

AIResponse OllamaClient::generateReply(const std::vector<Message>& requestMessages) const
{
    nlohmann::json requestJson;
    requestJson["model"] = config.modelName;
    requestJson["messages"] = nlohmann::json::array();
    requestJson["stream"] = config.stream;

    for (const Message& message : requestMessages)
    {
        requestJson["messages"].push_back({
            {"role", roleToString(message.role)},
            {"content", message.content}
        });
    }

    httplib::Client client(config.serverUrl);

    const int timeoutSeconds = normalizeTimeoutSeconds(config.timeoutSeconds);
    client.set_connection_timeout(timeoutSeconds, 0);
    client.set_read_timeout(timeoutSeconds, 0);
    client.set_write_timeout(timeoutSeconds, 0);

    const std::string requestBody = requestJson.dump();
    auto response = client.Post(config.apiPath, requestBody, "application/json");

    if (!response)
    {
        return makeNetworkErrorResponse(httplib::to_string(response.error()));
    }

    if (response->status < 200 || response->status >= 300)
    {
        return makeHttpErrorResponse(response->status, response->body);
    }

    nlohmann::json responseJson;
    try
    {
        responseJson = nlohmann::json::parse(response->body);
    }
    catch (const nlohmann::json::parse_error&)
    {
        return makeFailureResponse("Ollama 응답 JSON을 파싱할 수 없습니다.");
    }

    if (!responseJson.contains("message") ||
        !responseJson["message"].is_object() ||
        !responseJson["message"].contains("content") ||
        !responseJson["message"]["content"].is_string())
    {
        return makeFailureResponse("Ollama 응답에 message.content 문자열이 없습니다.");
    }

    return makeSuccessResponse(responseJson["message"]["content"].get<std::string>());
}

std::string OllamaClient::roleToString(MessageRole role) const
{
    switch (role)
    {
    case MessageRole::System:
        return "system";
    case MessageRole::User:
        return "user";
    case MessageRole::Assistant:
        return "assistant";
    }

    return "user";
}

AIResponse OllamaClient::makeNetworkErrorResponse(const std::string& errorText) const
{
    if (errorText.find("ConnectionTimeout") != std::string::npos ||
        errorText.find("Read") != std::string::npos ||
        errorText.find("Write") != std::string::npos)
    {
        return makeFailureResponse("Ollama 서버 응답 시간이 초과되었습니다: " + errorText);
    }

    return makeFailureResponse("Ollama 서버에 연결할 수 없습니다: " + errorText);
}

AIResponse OllamaClient::makeHttpErrorResponse(int statusCode, const std::string& body) const
{
    const std::string ollamaErrorMessage = extractOllamaErrorMessage(body);

    if (containsModelNotFoundText(ollamaErrorMessage) || containsModelNotFoundText(body))
    {
        return makeFailureResponse(
            "Ollama 모델을 찾을 수 없습니다. modelName 설정을 확인하세요: " + config.modelName);
    }

    std::string message = "Ollama HTTP 요청이 실패했습니다. status=" + std::to_string(statusCode);
    if (!ollamaErrorMessage.empty())
    {
        message += ", error=" + ollamaErrorMessage;
    }

    return makeFailureResponse(message);
}

std::string OllamaClient::extractOllamaErrorMessage(const std::string& body) const
{
    try
    {
        const nlohmann::json errorJson = nlohmann::json::parse(body);
        if (errorJson.contains("error") && errorJson["error"].is_string())
        {
            return errorJson["error"].get<std::string>();
        }
    }
    catch (const nlohmann::json::parse_error&)
    {
        return body;
    }

    return "";
}
