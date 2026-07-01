#include "ChatApp.h"
#include "ContextBuilder.h"
#include "ConversationHistory.h"
#include "ConsoleUI.h"
#include "FakeAIClient.h"

int main()
{
    // main 함수는 프로그램 시작점입니다.
    // 여기서는 필요한 객체를 만들고 ChatApp에 전달하는 조립 역할만 합니다.
    ConversationHistory history;
    ConsoleUI ui;
    ContextBuilder contextBuilder;
    FakeAIClient aiClient;

    // ChatApp은 IAIClient 인터페이스를 통해 AI를 사용하므로
    // 나중에 FakeAIClient 대신 OllamaClient를 전달해도 실행 루프는 바뀌지 않습니다.
    ChatApp app(history, ui, contextBuilder, aiClient);
    app.run();

    return 0;
}
