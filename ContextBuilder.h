#ifndef CONTEXT_BUILDER_H
#define CONTEXT_BUILDER_H

#include "Message.h"

#include <cstddef>
#include <vector>

// ContextBuilder는 전체 대화 기록에서 AI 요청에 보낼 메시지 목록을 만듭니다.
// ConversationHistory를 직접 수정하지 않고, 요청용 std::vector<Message>를 새로 반환합니다.
class ContextBuilder
{
public:
    // 기본값은 최근 User/Assistant 메시지 10개입니다.
    ContextBuilder();

    // maxContextMessages는 요청에 포함할 최근 User/Assistant 메시지 개수입니다.
    explicit ContextBuilder(std::size_t maxContextMessages);

    // 전체 메시지 목록에서 요청용 컨텍스트를 만들어 반환합니다.
    // System 메시지는 모두 앞쪽에 포함하고, User/Assistant 메시지는 최근 N개만 포함합니다.
    std::vector<Message> build(const std::vector<Message>& allMessages) const;

    // 현재 최대 컨텍스트 메시지 개수를 조회합니다.
    std::size_t getMaxContextMessages() const;

    // 최대 컨텍스트 메시지 개수를 변경합니다.
    void setMaxContextMessages(std::size_t maxContextMessages);

private:
    std::size_t maxContextMessages;
};

#endif
