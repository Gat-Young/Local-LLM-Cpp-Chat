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
    // const 참조이므로 복사 비용은 줄이고, 외부에서 내용을 수정하지 못하게 합니다.
    const std::vector<Message>& getMessages() const;

    // 대화 기록을 모두 비웁니다.
    void clear();

    // 외부에서 준비한 메시지 목록으로 현재 대화 기록을 통째로 교체합니다.
    // 파일에서 불러온 기록을 적용할 때 사용할 수 있습니다.
    void replaceMessages(const std::vector<Message>& newMessages);

private:
    // 메시지는 입력된 순서대로 vector에 저장됩니다.
    // private으로 감춰 외부 코드가 기록을 마음대로 바꾸지 못하게 합니다.
    std::vector<Message> messages;
};

#endif
