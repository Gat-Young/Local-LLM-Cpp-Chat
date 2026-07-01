#include "AppConfigLoader.h"

#include <algorithm>
#include <cctype>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace
{
std::string trim(const std::string& text)
{
    const auto first = std::find_if(text.begin(), text.end(), [](unsigned char character) {
        return std::isspace(character) == 0;
    });

    const auto last = std::find_if(text.rbegin(), text.rend(), [](unsigned char character) {
        return std::isspace(character) == 0;
    }).base();

    if (first >= last)
    {
        return "";
    }

    return std::string(first, last);
}

std::string toLower(std::string text)
{
    std::transform(text.begin(), text.end(), text.begin(), [](unsigned char character) {
        return static_cast<char>(std::tolower(character));
    });

    return text;
}

bool parsePositiveInt(const std::string& text, int& value)
{
    try
    {
        std::size_t parsedLength = 0;
        const int parsedValue = std::stoi(text, &parsedLength);

        if (parsedLength != text.size() || parsedValue <= 0)
        {
            return false;
        }

        value = parsedValue;
        return true;
    }
    catch (const std::invalid_argument&)
    {
        return false;
    }
    catch (const std::out_of_range&)
    {
        return false;
    }
}

bool parseBool(const std::string& text, bool& value)
{
    const std::string normalized = toLower(trim(text));

    if (normalized == "true" || normalized == "1")
    {
        value = true;
        return true;
    }

    if (normalized == "false" || normalized == "0")
    {
        value = false;
        return true;
    }

    return false;
}

void addWarning(AppConfigLoadResult& result, int lineNumber, const std::string& message)
{
    std::ostringstream warning;
    warning << "line " << lineNumber << ": " << message;
    result.warnings.push_back(warning.str());
}
}

AppConfigLoadResult AppConfigLoader::load(const std::string& filePath) const
{
    AppConfigLoadResult result;

    // 설정 파일이 없으면 기본값을 그대로 사용합니다.
    if (!std::filesystem::exists(filePath))
    {
        return result;
    }

    std::ifstream inputFile(filePath);
    if (!inputFile.is_open())
    {
        result.success = false;
        result.errorMessage = "설정 파일을 열 수 없습니다: " + filePath;
        return result;
    }

    result.loadedFromFile = true;

    std::string line;
    int lineNumber = 0;

    while (std::getline(inputFile, line))
    {
        ++lineNumber;

        const std::string trimmedLine = trim(line);
        if (trimmedLine.empty() || trimmedLine[0] == '#' || trimmedLine[0] == ';')
        {
            continue;
        }

        const std::size_t separatorPosition = trimmedLine.find('=');
        if (separatorPosition == std::string::npos)
        {
            addWarning(result, lineNumber, "key=value 형식이 아니므로 무시했습니다.");
            continue;
        }

        const std::string key = trim(trimmedLine.substr(0, separatorPosition));
        const std::string value = trim(trimmedLine.substr(separatorPosition + 1));

        if (key == "serverUrl")
        {
            result.config.serverUrl = value;
        }
        else if (key == "apiPath")
        {
            result.config.apiPath = value;
        }
        else if (key == "modelName")
        {
            result.config.modelName = value;
        }
        else if (key == "maxContextMessages")
        {
            int parsedValue = 0;
            if (parsePositiveInt(value, parsedValue))
            {
                result.config.maxContextMessages = static_cast<std::size_t>(parsedValue);
            }
            else
            {
                addWarning(result, lineNumber, "maxContextMessages 값이 올바른 양수가 아니므로 기본값을 유지했습니다.");
            }
        }
        else if (key == "stream")
        {
            bool parsedValue = false;
            if (parseBool(value, parsedValue))
            {
                result.config.stream = parsedValue;
            }
            else
            {
                addWarning(result, lineNumber, "stream 값은 true, false, 1, 0 중 하나여야 하므로 기본값을 유지했습니다.");
            }
        }
        else if (key == "timeoutSeconds")
        {
            int parsedValue = 0;
            if (parsePositiveInt(value, parsedValue))
            {
                result.config.timeoutSeconds = parsedValue;
            }
            else
            {
                addWarning(result, lineNumber, "timeoutSeconds 값이 올바른 양수가 아니므로 기본값을 유지했습니다.");
            }
        }
        else
        {
            addWarning(result, lineNumber, "알 수 없는 설정 키이므로 무시했습니다: " + key);
        }
    }

    return result;
}
