#include <iostream>
#include <string>

int main()
{
    // 사용자에게 프로그램의 목적과 종료 방법을 알려줍니다.
    std::cout << "Console Chat Step 1" << std::endl;
    std::cout << "Enter a message. Type /exit to quit." << std::endl;

    // 사용자가 입력한 한 줄 전체를 저장할 문자열 변수입니다.
    std::string userInput;

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

        // 아직 AI 연동은 하지 않고, 사용자가 입력한 내용을 그대로 다시 출력합니다.
        std::cout << "You said: " << userInput << std::endl;
    }

    // 프로그램이 정상적으로 종료됨을 알려줍니다.
    std::cout << "Chat ended." << std::endl;

    return 0;
}
