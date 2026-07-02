#include "ChatApp.h"
#include "AppConfigLoader.h"
#include "ContextBuilder.h"
#include "ConversationHistory.h"
#include "ConversationRepository.h"
#include "ConsoleUI.h"
#include "OllamaClient.h"

#include <chrono>
#include <string>
#include <thread>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#else
#include <cstdlib>
#endif

namespace
{
#ifdef _WIN32
bool startHiddenProcess(const std::string& commandLineText)
{
    STARTUPINFOA startupInfo{};
    startupInfo.cb = sizeof(startupInfo);

    PROCESS_INFORMATION processInfo{};
    std::vector<char> commandLine(commandLineText.begin(), commandLineText.end());
    commandLine.push_back('\0');

    const BOOL started = CreateProcessA(
        nullptr,
        commandLine.data(),
        nullptr,
        nullptr,
        FALSE,
        CREATE_NO_WINDOW,
        nullptr,
        nullptr,
        &startupInfo,
        &processInfo);

    if (!started)
    {
        return false;
    }

    CloseHandle(processInfo.hThread);
    CloseHandle(processInfo.hProcess);
    return true;
}
#endif

bool startOllamaServer()
{
#ifdef _WIN32
    // Windows에서는 별도 콘솔 창을 띄우지 않고 ollama serve를 백그라운드로 시작합니다.
    if (startHiddenProcess("ollama serve"))
    {
        return true;
    }

    // PATH에 ollama가 없을 때 Windows 기본 설치 위치도 한 번 더 시도합니다.
    char localAppData[MAX_PATH]{};
    const DWORD length = GetEnvironmentVariableA("LOCALAPPDATA", localAppData, MAX_PATH);
    if (length > 0 && length < MAX_PATH)
    {
        const std::string ollamaPath =
            std::string("\"") + localAppData + "\\Programs\\Ollama\\ollama.exe\" serve";
        return startHiddenProcess(ollamaPath);
    }

    return false;
#else
    // Unix 계열에서는 백그라운드 실행으로 서버를 시작합니다.
    return std::system("ollama serve >/dev/null 2>&1 &") == 0;
#endif
}
}

int main()
{
    // main 함수는 프로그램을 구성하는 조립 지점입니다.
    // 설정을 읽고, 필요한 객체를 만든 뒤, ChatApp에 전달하고 실행합니다.
    AppConfigLoader configLoader;
    AppConfigLoadResult configResult = configLoader.load("config.ini");
    AppConfig config = configResult.config;

    ConsoleUI ui;
    if (!configResult.success)
    {
        ui.printError(configResult.errorMessage);
    }

    for (const std::string& warning : configResult.warnings)
    {
        ui.printError("Config warning: " + warning);
    }

    const bool ollamaStartRequested = startOllamaServer();
    if (!ollamaStartRequested)
    {
        ui.printError("Ollama 자동 실행에 실패했습니다. Ollama가 설치되어 있고 PATH에 등록되어 있는지 확인하세요.");
    }

    if (ollamaStartRequested)
    {
        // 막 시작한 Ollama 서버가 포트를 열 시간을 짧게 줍니다.
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    ConversationHistory history;
    history.addMessage(MessageRole::System, "너는 친절한 로컬 AI assistant입니다.");

    ContextBuilder contextBuilder(config.maxContextMessages);
    OllamaClient aiClient(config);
    ConversationRepository repository;

    ChatApp app(history, ui, contextBuilder, aiClient, repository);
    app.run();

    return 0;
}
