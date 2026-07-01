#ifndef FAKE_AI_CLIENT_H
#define FAKE_AI_CLIENT_H

#include "IAIClient.h"

#include <string>
#include <vector>

// FakeAIClient는 실제 네트워크 요청 없이 고정 응답을 돌려주는 테스트용 AI입니다.
// ChatApp 전체 흐름을 Ollama 연결 전에 먼저 검증할 때 사용합니다.
class FakeAIClient : public IAIClient
{
public:
    // 기본 고정 응답으로 성공 결과를 반환하는 FakeAIClient를 만듭니다.
    FakeAIClient();

    // 테스트에서 원하는 고정 응답 문자열을 지정할 수 있습니다.
    explicit FakeAIClient(const std::string& fixedResponse);

    // 실패 상황 테스트를 위해 성공/실패 동작과 실패 메시지를 직접 지정할 수 있습니다.
    FakeAIClient(const std::string& fixedResponse, bool shouldFail, const std::string& failureMessage);

    // 요청용 메시지 목록을 받지만, FakeAIClient는 네트워크를 쓰지 않고 고정 결과를 반환합니다.
    AIResponse generateReply(const std::vector<Message>& requestMessages) const override;

    // 테스트 중 실패 응답을 내도록 설정합니다.
    void setShouldFail(bool shouldFail);

    // 실패 응답일 때 사용할 오류 메시지를 설정합니다.
    void setFailureMessage(const std::string& failureMessage);

private:
    std::string fixedResponse;
    bool shouldFail;
    std::string failureMessage;
};

#endif
