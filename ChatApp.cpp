#include "ChatApp.h"

void ChatApp::run()
{
    ui.printWelcome();

    // 사용자가 입력한 한 줄 전체를 저장할 문자열 변수입니다.
    std::string userInput;

    // /exit이 입력될 때까지 계속해서 메시지를 입력받습니다.
    while (true)
    {
        ui.printPrompt();

        // ConsoleUI가 공백을 포함한 한 줄 입력을 처리합니다.
        if (!ui.readLine(userInput))
        {
            break;
        }

        // 종료 명령어는 대화 기록에 저장하지 않고 프로그램을 끝냅니다.
        if (isExitCommand(userInput))
        {
            break;
        }

        // 사용자가 입력한 내용을 User 역할의 메시지로 저장합니다.
        history.addMessage(MessageRole::User, userInput);

        // 아직 실제 AI는 없으므로 고정된 문자열을 Assistant 응답으로 사용합니다.
        std::string assistantReply = createTemporaryAssistantReply();

        // 고정 응답도 Assistant 역할의 메시지로 대화 기록에 저장합니다.
        history.addMessage(MessageRole::Assistant, assistantReply);

        // 콘솔에서는 Assistant가 보낸 응답을 사용자에게 보여줍니다.
        ui.printAssistantMessage(assistantReply);
    }

    ui.printGoodbye();
}

bool ChatApp::isExitCommand(const std::string& input) const
{
    return input == "/exit";
}

std::string ChatApp::createTemporaryAssistantReply() const
{
    return "임시 응답입니다.";
}
