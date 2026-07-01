#ifndef CONSOLE_UI_H
#define CONSOLE_UI_H

#include <string>

// ConsoleUI는 콘솔 입력과 출력을 담당하는 클래스입니다.
// ChatApp은 화면 출력 방식을 직접 알 필요 없이 이 클래스를 통해 입출력합니다.
class ConsoleUI
{
public:
    // Windows 콘솔에서 한글이 깨지지 않도록 필요한 설정을 준비합니다.
    ConsoleUI();

    // 프로그램 시작 안내 문구를 출력합니다.
    void printWelcome() const;

    // 사용자가 입력할 차례임을 보여주는 프롬프트를 출력합니다.
    void printPrompt() const;

    // 한 줄 전체를 입력받습니다. 입력 실패나 종료가 발생하면 false를 돌려줍니다.
    bool readLine(std::string& input) const;

    // Assistant 역할의 응답을 콘솔에 출력합니다.
    void printAssistantMessage(const std::string& message) const;

    // 프로그램 종료 안내 문구를 출력합니다.
    void printGoodbye() const;

private:
    // 콘솔 인코딩처럼 ConsoleUI 내부에서만 필요한 준비 작업입니다.
    void configureConsoleEncoding() const;
};

#endif
