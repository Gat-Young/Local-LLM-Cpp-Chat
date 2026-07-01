#ifndef OLLAMA_CLIENT_H
#define OLLAMA_CLIENT_H

#include "AppConfig.h"
#include "IAIClient.h"

#include <string>
#include <vector>

// OllamaClient는 Ollama 로컬 서버의 /api/chat HTTP API를 호출하는 AI 클라이언트입니다.
// 콘솔 입력/출력은 하지 않고, 성공 또는 실패를 AIResponse로만 반환합니다.
class OllamaClient : public IAIClient
{
public:
    // AppConfig에 들어 있는 서버 주소, API 경로, 모델명, stream, timeout 설정을 사용합니다.
    explicit OllamaClient(const AppConfig& config);

    // 요청용 메시지 목록을 Ollama messages 배열로 변환해 POST /api/chat에 전달합니다.
    AIResponse generateReply(const std::vector<Message>& requestMessages) const override;

private:
    // MessageRole 값을 Ollama API가 요구하는 role 문자열로 변환합니다.
    std::string roleToString(MessageRole role) const;

    // cpp-httplib의 네트워크 오류를 사용자에게 보여줄 실패 메시지로 바꿉니다.
    AIResponse makeNetworkErrorResponse(const std::string& errorText) const;

    // HTTP 상태 코드와 응답 body를 보고 실패 AIResponse를 만듭니다.
    AIResponse makeHttpErrorResponse(int statusCode, const std::string& body) const;

    // Ollama 오류 body에서 error 필드가 있으면 추출합니다.
    std::string extractOllamaErrorMessage(const std::string& body) const;

    AppConfig config;
};

#endif
