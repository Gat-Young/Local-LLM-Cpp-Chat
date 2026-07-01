#include "ChatApp.h"
#include "ContextBuilder.h"
#include "ConversationHistory.h"
#include "ConsoleUI.h"
#include "FakeAIClient.h"
#include "OllamaClient.h"
#include "AppConfigLoader.h"
#include "ConversationRepository.h"

int main()
{
    AppConfigLoader configLoader;
    AppConfigLoadResult configResult = configLoader.load("config.ini");
    AppConfig config = configResult.config;

    ConsoleUI ui;
    ConversationHistory history;
    history.addMessage(MessageRole::System, "너는 친절한 로컬 AI assistant입니다.");

    ContextBuilder contextBuilder(config.maxContextMessages);
    //FakeAIClient aiClient;
    OllamaClient aiClient(config);
    ConversationRepository repository;

    ChatApp app(history, ui, contextBuilder, aiClient, repository);
    app.run();

    return 0;
}
