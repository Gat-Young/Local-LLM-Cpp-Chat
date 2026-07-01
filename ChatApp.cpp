#include "ChatApp.h"

#include <algorithm>
#include <cctype>
#include <vector>

namespace
{
std::string trimWhitespace(const std::string& input)
{
    const auto firstNonSpace = std::find_if(input.begin(), input.end(), [](unsigned char character) {
        return std::isspace(character) == 0;
    });

    const auto lastNonSpace = std::find_if(input.rbegin(), input.rend(), [](unsigned char character) {
        return std::isspace(character) == 0;
    }).base();

    if (firstNonSpace >= lastNonSpace)
    {
        return "";
    }

    return std::string(firstNonSpace, lastNonSpace);
}
}

ChatApp::ChatApp(
    ConversationHistory& history,
    ConsoleUI& ui,
    ContextBuilder& contextBuilder,
    IAIClient& aiClient)
    : history(history),
      ui(ui),
      contextBuilder(contextBuilder),
      aiClient(aiClient)
{
}

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

        // 빈 입력은 대화 기록에 저장하지 않고 AI에게도 보내지 않습니다.
        if (isEmptyInput(userInput))
        {
            continue;
        }

        // 종료 명령어는 대화 기록에 저장하지 않고 프로그램을 끝냅니다.
        if (isExitCommand(userInput))
        {
            break;
        }

        // 사용자가 입력한 내용을 User 역할의 메시지로 저장합니다.
        history.addMessage(MessageRole::User, userInput);

        // 전체 대화 기록에서 AI 요청에 보낼 메시지 목록만 따로 만듭니다.
        std::vector<Message> requestMessages = contextBuilder.build(history.getMessages());

        // ChatApp은 구체 AI 구현을 모르고 IAIClient 인터페이스를 통해 응답을 요청합니다.
        AIResponse response = aiClient.generateReply(requestMessages);

        // 실패한 응답은 Assistant 메시지로 저장하지 않고 오류만 보여줍니다.
        if (!response.success)
        {
            ui.printError(response.errorMessage);
            continue;
        }

        // 성공한 응답만 Assistant 역할의 메시지로 대화 기록에 저장합니다.
        history.addMessage(MessageRole::Assistant, response.content);

        // 콘솔에서는 Assistant가 보낸 응답을 사용자에게 보여줍니다.
        ui.printAssistantMessage(response.content);
    }

    ui.printGoodbye();
}

bool ChatApp::isEmptyInput(const std::string& input) const
{
    return std::all_of(input.begin(), input.end(), [](unsigned char character) {
        return std::isspace(character) != 0;
    });
}

bool ChatApp::isExitCommand(const std::string& input) const
{
    return trimWhitespace(input) == "/exit";
}
