#ifndef APP_CONFIG_H
#define APP_CONFIG_H

#include <cstddef>
#include <string>

// AppConfig는 프로그램 실행 설정을 담는 단순 데이터 구조입니다.
// 기본값을 멤버 초기값으로 둬서 설정 파일이 없어도 바로 실행할 수 있습니다.
struct AppConfig
{
    std::string serverUrl = "http://localhost:11434";
    std::string apiPath = "/api/chat";

    // 실제 설치한 Ollama 모델명으로 쉽게 바꿀 수 있도록 문자열로 보관합니다.
    std::string modelName = "llama3.2";

    // ContextBuilder가 사용할 최근 User/Assistant 메시지 개수입니다.
    std::size_t maxContextMessages = 10;

    // 처음에는 단일 JSON 응답만 받기 위해 false를 기본값으로 둡니다.
    bool stream = false;

    // Ollama 응답을 기다릴 최대 시간입니다.
    int timeoutSeconds = 30;
};

#endif
