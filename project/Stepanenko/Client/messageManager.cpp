#include "messageManager.h"
#include "chatClient.h"
#include "worker.h"

MessageManager::MessageManager()
{
    chatClient_ = ChatClientPtr(new ChatClient("127.0.0.1", "1122"));
}

void MessageManager::initializeSession()
{
    chatClient_->start();

    Worker::instance()->start();
    LOG_INFO("Thread started!!!");

    chatClient_->getUsersListFromServer();
    chatClient_->askNameAndRegister();
}

void MessageManager::comunicateWithUser()
{
    std::cout << "Use COMMAND to control client" << std::endl;
    std::cout << "Use COMMAND HELP for help" << std::endl;
    std::string message;
    std::string firstWord, secondWord, thirdWord;
    while(true)
    {
        getline(std::cin, message);
        std::stringstream tempStream(message);
        tempStream >> firstWord;
        if (firstWord == "COMMAND")
        {
            tempStream >> secondWord;
            if (secondWord == "HELP")
            {
                showHelp();
            }
            else if (secondWord == "SERVER_USERS")
            {
                chatClient_->getUsersListFromServer();
            }
            else if (secondWord == "START_CHAT")
            {
                tempStream >> thirdWord;
                chatClient_->connectToUser(thirdWord);
            }
            else if (secondWord == "DISCONNECT")
            {
                chatClient_->disconnect();
            }
            else if (secondWord == "EXIT")
            {
                std::cout << "Good by!" << std::endl;
                break;
            }
            else
            {
                std::cout << "I don't know this command! Use COMMAND HELP for command list" << std::endl;
            }
        }
        else if (chatClient_->isInChat())
        {
            chatClient_->sendChatMessage(message);
        }
        else
        {
            std::cout << "You can't write messages while you are not in the chat room!" << std::endl;
        }

    }
}

void MessageManager::closeSession()
{
    if (chatClient_->isLoggedIn())
    {
        chatClient_->disconnect();
    }
    chatClient_->stop();
    Worker::instance()->join();
    LOG_INFO("Thread finished!");
}

void MessageManager::showHelp()
{
    std::cout << "----------------------------------------------------------------------" << std::endl;
    std::cout << "COMMAND HELP - this output" << std::endl;
    std::cout << "COMMAND SERVER_USERS - send request to server for updating local users" << std::endl;
    std::cout << "COMMAND START_CHAT USER - begin chat with choosen user" << std::endl;
    std::cout << "COMMAND DISCONNECT - disconnect from server by choosen username" << std::endl;
    std::cout << "COMMAND EXIT - exit from program" << std::endl;
    std::cout << "----------------------------------------------------------------------" << std::endl << std::endl;
}

