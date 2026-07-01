#ifndef AI_RESPONSE_H
#define AI_RESPONSE_H

#include <string>

// AI 응답 생성 결과를 표현하는 단순 데이터 구조입니다.
// 성공하면 success는 true이고 content에 Assistant 응답이 들어갑니다.
// 실패하면 success는 false이고 errorMessage에 실패 이유가 들어갑니다.
struct AIResponse
{
    bool success;
    std::string content;
    std::string errorMessage;
};

#endif
