#ifndef APP_CONFIG_LOADER_H
#define APP_CONFIG_LOADER_H

#include "AppConfig.h"

#include <string>
#include <vector>

// AppConfigLoader가 설정 파일을 읽은 결과입니다.
// 설정 파일이 없거나 일부 값이 잘못되어도 기본값으로 계속 실행할 수 있게
// config와 warnings를 함께 돌려줍니다.
struct AppConfigLoadResult
{
    AppConfig config;
    bool success = true;
    bool loadedFromFile = false;
    std::string errorMessage;
    std::vector<std::string> warnings;
};

// AppConfigLoader는 INI 형식 설정 파일을 읽어 AppConfig를 만듭니다.
// ChatApp, OllamaClient, ContextBuilder는 설정 파일 형식을 몰라도 됩니다.
class AppConfigLoader
{
public:
    // 설정 파일 경로를 받아 AppConfigLoadResult를 반환합니다.
    // 파일이 없으면 기본값 AppConfig를 성공 결과로 반환합니다.
    AppConfigLoadResult load(const std::string& filePath) const;
};

#endif
