#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>

// 메시지가 어떤 역할인지 구분하기 위한 열거형입니다.
// enum class를 사용하면 MessageRole::User처럼 역할 이름을 명확하게 쓸 수 있습니다.
enum class MessageRole
{
    System,
    User,
    Assistant
};

// 채팅 메시지 하나를 표현하는 단순 데이터 구조입니다.
// role은 메시지를 보낸 주체를 나타내고, content는 실제 문자열 내용을 저장합니다.
struct Message
{
    MessageRole role;
    std::string content;
};

#endif
