#include <iostream>
#include <string>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#include <clocale>
#endif

// 메시지가 어떤 역할인지 구분하기 위한 열거형입니다.
// enum class를 사용하면 MessageRole::User처럼 역할 이름을 명확하게 쓸 수 있습니다.
enum class MessageRole
{
    System,
    User,
    Assistant
};

// 채팅 메시지 하나를 표현하는 구조체입니다.
// role은 메시지를 보낸 주체를 나타내고, content는 실제 문자열 내용을 저장합니다.
struct Message
{
    MessageRole role;
    std::string content;
};

int main()
{
#ifdef _WIN32
    // Windows 콘솔은 기본 문자 인코딩이 UTF-8이 아닐 수 있습니다.
    // 한글 문자열이 깨지지 않도록 콘솔의 입력과 출력을 UTF-8로 맞춥니다.
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    std::setlocale(LC_ALL, ".UTF-8");
#endif

    // 사용자에게 프로그램의 목적과 종료 방법을 알려줍니다.
    std::cout << "Console Chat Step 2" << std::endl;
    std::cout << "Enter a message. Type /exit to quit." << std::endl;

    // 사용자가 입력한 한 줄 전체를 저장할 문자열 변수입니다.
    std::string userInput;

    // 전체 대화 기록을 순서대로 저장하는 벡터입니다.
    // 포인터나 동적 할당을 직접 사용하지 않고 Message 값을 그대로 저장합니다.
    std::vector<Message> conversationHistory;

    // /exit이 입력될 때까지 계속해서 메시지를 입력받습니다.
    while (true)
    {
        // 입력할 차례임을 보여주는 간단한 프롬프트입니다.
        std::cout << "> ";

        // std::getline은 공백이 포함된 문장도 한 줄 전체로 입력받을 수 있습니다.
        std::getline(std::cin, userInput);

        // 입력 도중 오류가 나거나 입력이 끝난 경우 반복을 종료합니다.
        if (!std::cin)
        {
            break;
        }

        // 사용자가 /exit을 입력하면 채팅 반복을 끝냅니다.
        if (userInput == "/exit")
        {
            break;
        }

        // 사용자가 입력한 내용을 User 역할의 메시지로 만들어 대화 기록에 저장합니다.
        Message userMessage{MessageRole::User, userInput};
        conversationHistory.push_back(userMessage);

        // 아직 실제 AI는 없으므로 고정된 문자열을 Assistant 응답으로 사용합니다.
        std::string assistantReply = "임시 응답입니다.";

        // 고정 응답도 Assistant 역할의 메시지로 만들어 대화 기록에 저장합니다.
        Message assistantMessage{MessageRole::Assistant, assistantReply};
        conversationHistory.push_back(assistantMessage);

        // 콘솔에서는 Assistant가 보낸 응답만 사용자에게 보여줍니다.
        std::cout << "Assistant: " << assistantReply << std::endl;
    }

    // 프로그램이 정상적으로 종료됨을 알려줍니다.
    std::cout << "Chat ended." << std::endl;

    return 0;
}
