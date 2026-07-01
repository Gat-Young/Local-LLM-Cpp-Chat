#include "ConversationRepository.h"

#include <filesystem>
#include <fstream>

#if __has_include(<nlohmann/json.hpp>)
#include <nlohmann/json.hpp>
#elif __has_include("external/json/json.hpp")
#include "external/json/json.hpp"
#else
#error "nlohmann/json header not found. Install nlohmann-json or place json.hpp at external/json/json.hpp."
#endif

namespace
{
ConversationSaveResult makeSaveSuccess()
{
    return ConversationSaveResult{true, ConversationRepositoryError::None, ""};
}

ConversationSaveResult makeSaveError(ConversationRepositoryError errorType, const std::string& errorMessage)
{
    return ConversationSaveResult{false, errorType, errorMessage};
}

ConversationLoadResult makeLoadError(ConversationRepositoryError errorType, const std::string& errorMessage)
{
    ConversationLoadResult result;
    result.success = false;
    result.errorType = errorType;
    result.errorMessage = errorMessage;
    return result;
}
}

ConversationSaveResult ConversationRepository::save(
    const std::string& filePath,
    const std::vector<Message>& messages) const
{
    nlohmann::json root;
    root["messages"] = nlohmann::json::array();

    for (const Message& message : messages)
    {
        root["messages"].push_back({
            {"role", roleToString(message.role)},
            {"content", message.content}
        });
    }

    std::ofstream outputFile(filePath);
    if (!outputFile.is_open())
    {
        return makeSaveError(
            ConversationRepositoryError::FileOpenFailed,
            "대화 저장 파일을 열 수 없습니다: " + filePath);
    }

    outputFile << root.dump(4);

    if (!outputFile.good())
    {
        return makeSaveError(
            ConversationRepositoryError::FileWriteFailed,
            "대화 저장 파일에 쓰는 중 오류가 발생했습니다: " + filePath);
    }

    return makeSaveSuccess();
}

ConversationLoadResult ConversationRepository::load(const std::string& filePath) const
{
    if (!std::filesystem::exists(filePath))
    {
        return makeLoadError(
            ConversationRepositoryError::FileNotFound,
            "대화 저장 파일을 찾을 수 없습니다: " + filePath);
    }

    std::ifstream inputFile(filePath);
    if (!inputFile.is_open())
    {
        return makeLoadError(
            ConversationRepositoryError::FileOpenFailed,
            "대화 저장 파일을 열 수 없습니다: " + filePath);
    }

    nlohmann::json root;
    try
    {
        inputFile >> root;
    }
    catch (const nlohmann::json::parse_error&)
    {
        return makeLoadError(
            ConversationRepositoryError::JsonParseFailed,
            "대화 저장 파일의 JSON 형식이 올바르지 않습니다: " + filePath);
    }

    if (!root.is_object() || !root.contains("messages") || !root["messages"].is_array())
    {
        return makeLoadError(
            ConversationRepositoryError::InvalidJsonFormat,
            "대화 저장 파일에는 messages 배열이 있어야 합니다: " + filePath);
    }

    ConversationLoadResult result;
    result.success = true;
    result.errorType = ConversationRepositoryError::None;

    for (const nlohmann::json& item : root["messages"])
    {
        if (!item.is_object() || !item.contains("role") || !item["role"].is_string())
        {
            return makeLoadError(
                ConversationRepositoryError::InvalidJsonFormat,
                "각 메시지에는 문자열 role 값이 있어야 합니다.");
        }

        if (!item.contains("content") || !item["content"].is_string())
        {
            return makeLoadError(
                ConversationRepositoryError::MissingContent,
                "각 메시지에는 문자열 content 값이 있어야 합니다.");
        }

        MessageRole role = MessageRole::User;
        if (!tryParseRole(item["role"].get<std::string>(), role))
        {
            return makeLoadError(
                ConversationRepositoryError::InvalidRole,
                "role 값은 system, user, assistant 중 하나여야 합니다.");
        }

        result.messages.push_back(Message{role, item["content"].get<std::string>()});
    }

    return result;
}

std::string ConversationRepository::roleToString(MessageRole role) const
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

bool ConversationRepository::tryParseRole(const std::string& roleText, MessageRole& role) const
{
    if (roleText == "system")
    {
        role = MessageRole::System;
        return true;
    }

    if (roleText == "user")
    {
        role = MessageRole::User;
        return true;
    }

    if (roleText == "assistant")
    {
        role = MessageRole::Assistant;
        return true;
    }

    return false;
}
