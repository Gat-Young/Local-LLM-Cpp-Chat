#ifndef IAI_CLIENT_H
#define IAI_CLIENT_H

#include "AIResponse.h"
#include "Message.h"

#include <vector>

// IAIClient는 AI 응답 생성 기능을 나타내는 인터페이스입니다.
// ChatApp은 FakeAIClient인지 OllamaClient인지 몰라도 이 인터페이스만 사용하면 됩니다.
class IAIClient
{
public:
    // 인터페이스를 상속해서 사용할 때 안전하게 정리되도록 virtual 소멸자를 둡니다.
    virtual ~IAIClient() = default;

    // 요청용 메시지 목록을 받아 AI 응답 결과를 돌려줍니다.
    // requestMessages는 읽기만 하므로 const 참조로 받아 불필요한 복사를 피합니다.
    virtual AIResponse generateReply(const std::vector<Message>& requestMessages) const = 0;
};

#endif
