#ifndef CONVERSATION_REPOSITORY_H
#define CONVERSATION_REPOSITORY_H

#include "Message.h"

#include <string>
#include <vector>

// ConversationRepository에서 발생할 수 있는 오류 종류입니다.
// 호출하는 쪽은 errorMessage를 사용자에게 보여주고, errorType으로 원인을 구분할 수 있습니다.
enum class ConversationRepositoryError
{
    None,
    FileNotFound,
    FileOpenFailed,
    FileWriteFailed,
    JsonParseFailed,
    InvalidJsonFormat,
    InvalidRole,
    MissingContent
};

// 대화 기록 저장 결과입니다.
struct ConversationSaveResult
{
    bool success = false;
    ConversationRepositoryError errorType = ConversationRepositoryError::None;
    std::string errorMessage;
};

// 대화 기록 불러오기 결과입니다.
// 성공하면 messages에 복원된 메시지 목록이 들어갑니다.
struct ConversationLoadResult
{
    bool success = false;
    ConversationRepositoryError errorType = ConversationRepositoryError::None;
    std::string errorMessage;
    std::vector<Message> messages;
};

// ConversationRepository는 메시지 목록을 JSON 파일로 저장하고 다시 읽어오는 저장 전담 클래스입니다.
// ConversationHistory를 직접 알지 않고 std::vector<Message>만 다룹니다.
class ConversationRepository
{
public:
    // messages 배열을 가진 JSON 파일로 메시지 목록을 저장합니다.
    ConversationSaveResult save(const std::string& filePath, const std::vector<Message>& messages) const;

    // JSON 파일에서 메시지 목록을 읽어 std::vector<Message>로 복원합니다.
    ConversationLoadResult load(const std::string& filePath) const;

private:
    // MessageRole 값을 JSON에 저장할 문자열로 바꿉니다.
    std::string roleToString(MessageRole role) const;

    // JSON의 role 문자열을 MessageRole 값으로 바꿉니다.
    bool tryParseRole(const std::string& roleText, MessageRole& role) const;
};

#endif
