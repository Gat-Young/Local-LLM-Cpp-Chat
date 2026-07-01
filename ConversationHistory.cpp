#include "ConversationHistory.h"

void ConversationHistory::addMessage(const Message& message)
{
    messages.push_back(message);
}

void ConversationHistory::addMessage(MessageRole role, const std::string& content)
{
    Message message{role, content};
    addMessage(message);
}

const std::vector<Message>& ConversationHistory::getMessages() const
{
    return messages;
}

void ConversationHistory::clear()
{
    messages.clear();
}
