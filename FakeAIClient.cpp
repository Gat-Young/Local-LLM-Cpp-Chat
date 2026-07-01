#include "FakeAIClient.h"

FakeAIClient::FakeAIClient()
    : fixedResponse("임시 응답입니다."),
      shouldFail(false),
      failureMessage("Fake AI 응답 생성에 실패했습니다.")
{
}

FakeAIClient::FakeAIClient(const std::string& fixedResponse)
    : fixedResponse(fixedResponse),
      shouldFail(false),
      failureMessage("Fake AI 응답 생성에 실패했습니다.")
{
}

FakeAIClient::FakeAIClient(const std::string& fixedResponse, bool shouldFail, const std::string& failureMessage)
    : fixedResponse(fixedResponse),
      shouldFail(shouldFail),
      failureMessage(failureMessage)
{
}

AIResponse FakeAIClient::generateReply(const std::vector<Message>& requestMessages) const
{
    // FakeAIClient는 요청 내용을 실제로 분석하지 않습니다.
    // 매개변수 이름을 유지해 인터페이스 의미를 보여주고, 경고는 명시적으로 피합니다.
    (void)requestMessages;

    if (shouldFail)
    {
        return AIResponse{false, "", failureMessage};
    }

    return AIResponse{true, fixedResponse, ""};
}

void FakeAIClient::setShouldFail(bool shouldFail)
{
    this->shouldFail = shouldFail;
}

void FakeAIClient::setFailureMessage(const std::string& failureMessage)
{
    this->failureMessage = failureMessage;
}
