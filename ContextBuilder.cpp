#include "ContextBuilder.h"

ContextBuilder::ContextBuilder()
    : maxContextMessages(10)
{
}

ContextBuilder::ContextBuilder(std::size_t maxContextMessages)
    : maxContextMessages(maxContextMessages)
{
}

std::vector<Message> ContextBuilder::build(const std::vector<Message>& allMessages) const
{
    std::vector<Message> requestMessages;

    // System 메시지는 AI의 기본 지침 역할을 하므로 항상 요청 컨텍스트 앞쪽에 포함합니다.
    for (const Message& message : allMessages)
    {
        if (message.role == MessageRole::System)
        {
            requestMessages.push_back(message);
        }
    }

    // User/Assistant 메시지가 전체 몇 개인지 먼저 세어 최근 N개만 남길 기준을 정합니다.
    std::size_t userAssistantCount = 0;
    for (const Message& message : allMessages)
    {
        if (message.role == MessageRole::User || message.role == MessageRole::Assistant)
        {
            ++userAssistantCount;
        }
    }

    const std::size_t messagesToSkip =
        userAssistantCount > maxContextMessages ? userAssistantCount - maxContextMessages : 0;

    // 오래된 User/Assistant 메시지는 건너뛰고, 최근 N개는 원래 대화 순서대로 추가합니다.
    std::size_t skippedMessages = 0;
    for (const Message& message : allMessages)
    {
        if (message.role == MessageRole::System)
        {
            continue;
        }

        if (skippedMessages < messagesToSkip)
        {
            ++skippedMessages;
            continue;
        }

        requestMessages.push_back(message);
    }

    return requestMessages;
}

std::size_t ContextBuilder::getMaxContextMessages() const
{
    return maxContextMessages;
}

void ContextBuilder::setMaxContextMessages(std::size_t maxContextMessages)
{
    this->maxContextMessages = maxContextMessages;
}
