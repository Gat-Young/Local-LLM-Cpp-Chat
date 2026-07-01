#ifndef CHAT_APP_H
#define CHAT_APP_H

#include "ConversationHistory.h"
#include "ContextBuilder.h"
#include "ConsoleUI.h"
#include "IAIClient.h"
#include "OllamaClient.h"
#include "ConversationRepository.h"

#include <string>

// ChatApp은 프로그램 전체 흐름을 담당하는 클래스입니다.
// 입출력, 기록, 컨텍스트 생성, AI 응답 생성을 각각 담당 객체에 맡깁니다.
class ChatApp
{
public:
    // ChatApp은 필요한 객체를 직접 만들지 않고 외부에서 전달받아 사용합니다.
    // 이렇게 하면 FakeAIClient와 OllamaClient를 같은 방식으로 교체할 수 있습니다.
    ChatApp(
        ConversationHistory& history,
        ConsoleUI& ui,
        ContextBuilder& contextBuilder,
        OllamaClient& aiClient,
        ConversationRepository& repository
    );

    // 채팅 프로그램의 실행 루프를 시작합니다.
    void run();

private:
    // 아무 내용도 입력하지 않았는지 확인하는 내부 기능입니다.
    bool isEmptyInput(const std::string& input) const;

    // 종료 명령어인지 확인하는 내부 기능입니다.
    bool isExitCommand(const std::string& input) const;

    // 대화 기록 저장 담당 객체입니다. ChatApp이 소유하지 않고 빌려서 사용합니다.
    ConversationHistory& history;

    // 콘솔 입출력 담당 객체입니다. ChatApp이 소유하지 않고 빌려서 사용합니다.
    ConsoleUI& ui;

    // 전체 기록에서 AI 요청용 메시지 목록을 만드는 객체입니다.
    ContextBuilder& contextBuilder;

    // AI 응답 생성 인터페이스입니다. FakeAIClient나 OllamaClient가 이 자리에 들어올 수 있습니다.
    IAIClient& aiClient;

    ConversationRepository& repository;
};

#endif
