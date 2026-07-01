#ifndef CHAT_APP_H
#define CHAT_APP_H

#include "ConversationHistory.h"
#include "ConsoleUI.h"

#include <string>

// ChatApp은 프로그램 전체 흐름을 담당하는 클래스입니다.
// ConsoleUI로 입출력하고, ConversationHistory로 대화 기록을 저장합니다.
class ChatApp
{
public:
    // 채팅 프로그램의 실행 루프를 시작합니다.
    void run();

private:
    // 종료 명령어인지 확인하는 내부 기능입니다.
    bool isExitCommand(const std::string& input) const;

    // 실제 AI가 아직 없으므로 임시 Assistant 응답을 만듭니다.
    std::string createTemporaryAssistantReply() const;

    // 대화 기록 저장 담당 객체입니다. 값 타입 멤버로 직접 보관합니다.
    ConversationHistory history;

    // 콘솔 입출력 담당 객체입니다. 값 타입 멤버로 직접 보관합니다.
    ConsoleUI ui;
};

#endif
