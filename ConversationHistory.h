#ifndef CONVERSATION_HISTORY_H
#define CONVERSATION_HISTORY_H

#include "Message.h"

#include <string>
#include <vector>

// ConversationHistory는 전체 대화 기록을 관리하는 클래스입니다.
// public은 다른 클래스가 사용해도 되는 기능이고, private은 내부에서만 쓰는 데이터입니다.
class ConversationHistory
{
public:
    // 새 메시지를 대화 기록의 맨 뒤에 추가합니다.
    void addMessage(const Message& message);

    // 역할과 내용을 받아 Message를 만든 뒤 대화 기록에 추가합니다.
    void addMessage(MessageRole role, const std::string& content);

    // 저장된 전체 메시지 목록을 읽기 전용으로 돌려줍니다.
    const std::vector<Message>& getMessages() const;

    // 대화 기록을 모두 비웁니다.
    void clear();

private:
    // 메시지는 입력된 순서대로 vector에 저장됩니다.
    std::vector<Message> messages;
};

#endif
