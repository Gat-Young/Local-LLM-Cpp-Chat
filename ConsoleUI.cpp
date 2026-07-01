#include "ConsoleUI.h"

#include <iostream>

#ifdef _WIN32
#include <clocale>
#include <windows.h>
#endif

ConsoleUI::ConsoleUI()
{
    configureConsoleEncoding();
}

void ConsoleUI::printWelcome() const
{
    std::cout << "Console Chat Step 3" << std::endl;
    std::cout << "Enter a message. Type /exit to quit." << std::endl;
}

void ConsoleUI::printPrompt() const
{
    std::cout << "> ";
}

bool ConsoleUI::readLine(std::string& input) const
{
    std::getline(std::cin, input);
    return static_cast<bool>(std::cin);
}

void ConsoleUI::printAssistantMessage(const std::string& message) const
{
    std::cout << "Assistant: " << message << std::endl;
}

void ConsoleUI::printGoodbye() const
{
    std::cout << "Chat ended." << std::endl;
}

void ConsoleUI::configureConsoleEncoding() const
{
#ifdef _WIN32
    // Windows 콘솔은 기본 문자 인코딩이 UTF-8이 아닐 수 있습니다.
    // 한글 문자열이 깨지지 않도록 콘솔의 입력과 출력을 UTF-8로 맞춥니다.
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    std::setlocale(LC_ALL, ".UTF-8");
#endif
}
