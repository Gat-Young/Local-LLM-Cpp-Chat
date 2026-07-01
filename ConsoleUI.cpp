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
    std::cout << "Local AI Console Chat" << std::endl;
    std::cout << "Enter a message or type /help for commands." << std::endl;
}

void ConsoleUI::printPrompt() const
{
    std::cout << "> " << std::flush;
}

bool ConsoleUI::readLine(std::string& input) const
{
    std::getline(std::cin, input);
    return static_cast<bool>(std::cin);
}

void ConsoleUI::printMessage(const Message& message) const
{
    printMessage(message.role, message.content);
}

void ConsoleUI::printMessage(MessageRole role, const std::string& content) const
{
    std::cout << "[" << getRoleLabel(role) << "] " << content << std::endl;
}

void ConsoleUI::printAssistantMessage(const std::string& message) const
{
    printMessage(MessageRole::Assistant, message);
}

void ConsoleUI::printError(const std::string& message) const
{
    std::cerr << "[Error] " << message << std::endl;
}

void ConsoleUI::printHelp() const
{
    std::cout << "Commands:" << std::endl;
    std::cout << "  /exit  - End the program" << std::endl;
    std::cout << "  /clear - Clear the conversation" << std::endl;
    std::cout << "  /save  - Save the conversation" << std::endl;
    std::cout << "  /load  - Load the conversation" << std::endl;
    std::cout << "  /help  - Show this help" << std::endl;
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

std::string ConsoleUI::getRoleLabel(MessageRole role) const
{
    switch (role)
    {
    case MessageRole::System:
        return "System";
    case MessageRole::User:
        return "User";
    case MessageRole::Assistant:
        return "Assistant";
    }

    return "Unknown";
}
