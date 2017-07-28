#include "ClientChat.h"
#include <functional>
#include <Helper.h>
#include <algorithm>
#include <mutex>

ClientChat::ClientChat(std::string address, std::string port)
    : Client(address, port)
{

}

void ClientChat::onRead(ByteBufferPtr bufferPtr)
{
    CommandCode cCode = static_cast<CommandCode>((*bufferPtr)[0]);
    switch (cCode) {
    case CommandCode::LOGIN:
    {
        if( static_cast<bool>((*bufferPtr)[1]) )
        {
            std::cout << "Welcome!" << std::endl;
        }
        else
        {
            std::cout << "Oops! Something wrong: " << Helper::bufferToString(bufferPtr, 1) << std::endl;
        }
        break;
    }
    case CommandCode::USER_LIST:
    {
        std::cout << Helper::bufferToString(bufferPtr, 1) << std::endl;
        break;
    }
    case CommandCode::CONNECT_TO_USER:
    {
        std::string userName = Helper::bufferToString(bufferPtr, 1);
        std::cout << userName + " want star chat with you!" << std::endl
                  << "Use command CONFIRM_TO_START_CHAT [name]" << std::endl;

        auto it = std::find(usersWantToChat.begin(), usersWantToChat.end(), userName);
        if(it==usersWantToChat.end())
            usersWantToChat.emplace_back(userName);
        break;
    }
    case CommandCode::ANSWER_ON_REQUEST_TO_CONNECT:
    {
        std::string userName = Helper::bufferToString(bufferPtr, 2);
        if(static_cast<bool>( (*bufferPtr)[0]))
        {
            USER_INFO("User " + userName + " had confirmed your request to start chat");
        }
        else
        {
           USER_INFO("User " + userName + " hadn't confirmed your request to start chat");
        }
        break;
    }
    case CommandCode::SEND_MESSAGE:
    {
        std::cout << Helper::bufferToString(bufferPtr, 1) << std::endl;
        break;
    }
    case CommandCode::SHOW_QUEUE_USERS:
    {
        std::cout << Helper::bufferToString(bufferPtr, 1) << std::endl;
        break;
    }
    default:
        break;
    }
}

void ClientChat::execute(CommandCode cmd, ByteBufferPtr&& bufferPtr)
{
    switch (cmd)
    {
    case CommandCode::CONNECT_TO_USER:
    {
        connectToUser(bufferPtr);
        break;
    }
    case CommandCode::ANSWER_ON_REQUEST_TO_CONNECT:
    {
        answerOnRequestToConnect(bufferPtr);
        break;
    }
    case CommandCode::DISCONNECT_FROM_USER:
    {
        disconnectFromUser();
        break;
    }
    case CommandCode::LOGIN:
    {
        login(bufferPtr);
        break;
    }
    case CommandCode::LOGOUT:
    {
        logout();
        break;
    }
    case CommandCode::SEND_MESSAGE:
    {
        sendMessage(bufferPtr);
        break;
    }
    case CommandCode::USER_LIST:
    {
        getUserList();
        break;
    }
    case CommandCode::SHOW_QUEUE_USERS:
    {
        getQueueUsers();
        break;
    }
    case CommandCode::CONFIRM_TO_START_CHAT:
    {
        confirmToStarChat(bufferPtr);
        break;
    }
    case CommandCode::SING_UP:
    {
        singUp(bufferPtr);
        break;
    }
    default:
        break;
    }
}

void ClientChat::login(ByteBufferPtr name)
{
    if(name->empty())
    {
        std::cout << "Login is empty!" << std::endl;
        return;
    }

    Helper::insertCommandCode(name, CommandCode::LOGIN);
    write(name);
}

void ClientChat::logout()
{
    ByteBufferPtr buff = std::make_shared<ByteBuffer>();
    Helper::insertCommandCode(buff, CommandCode::LOGOUT);
    write(buff);
}

void ClientChat::sendMessage(ByteBufferPtr message)
{
    if(message->empty())
    {
        std::cout << "Message is empty!" << std::endl;
        return;
    }

    Helper::insertCommandCode(message, CommandCode::SEND_MESSAGE);
    write(message);
}

void ClientChat::getUserList()
{
    ByteBufferPtr buff = std::make_shared<ByteBuffer>();
    Helper::insertCommandCode(buff, CommandCode::USER_LIST);
    write(buff);

}

void ClientChat::connectToUser(ByteBufferPtr userName)
{
    if(userName->empty())
    {
        std::cout << "User's name is empty!" << std::endl;
        return;
    }

    Helper::insertCommandCode(userName, CommandCode::CONNECT_TO_USER);
    write(userName);

}

void ClientChat::disconnectFromUser()
{
    ByteBufferPtr buff = std::make_shared<ByteBuffer>();
    Helper::insertCommandCode(buff, CommandCode::DISCONNECT_FROM_USER);
    write(buff);
}

void ClientChat::answerOnRequestToConnect(ByteBufferPtr userNameAndAnswer)
{
    Helper::insertCommandCode(userNameAndAnswer, CommandCode::ANSWER_ON_REQUEST_TO_CONNECT);
    write(userNameAndAnswer);
}

void ClientChat::confirmToStarChat(ByteBufferPtr userName)
{
    std::string name = Helper::bufferToString(userName, 0);

    if(!isContainUserWhoWantChat(name))
    {
        std::cout << "Wrong user's name!" << std::endl;
        return;
    }

    int ind = -1;
    for(int i = 0; i<usersWantToChat.size(); i++)
    {
        if(usersWantToChat[i] == name)
        {
            ind = i;
            break;
        }
    }

    usersWantToChat.erase(usersWantToChat.begin()+ind);
    userName->emplace(userName->begin(), static_cast<uint8_t>(1));
    execute(CommandCode::ANSWER_ON_REQUEST_TO_CONNECT, std::move(userName));
}

void ClientChat::singUp(ByteBufferPtr userName)
{
    if(userName->empty())
    {
        std::cout << "User's name is empty!" << std::endl;
        return;
    }

    Helper::insertCommandCode(userName, CommandCode::SING_UP);
    write(userName);
}

void ClientChat::getQueueUsers()
{
    ByteBufferPtr buff = std::make_shared<ByteBuffer>();
    Helper::insertCommandCode(buff, CommandCode::SHOW_QUEUE_USERS);
    write(buff);
}

void ClientChat::printServerAnswer(ByteBufferPtr buffPtr)
{
    LOG_INFO(*buffPtr);
}

bool ClientChat::isContainUserWhoWantChat(const std::__cxx11::string &name)
{
    for(std::string user: usersWantToChat)
    {
        if(user == name)
        {
            return true;
        }
    }
    return false;
}

bool ClientChat::isEmptyQueueForChat()
{
    return usersWantToChat.empty();
}

void ClientChat::printQueueChat()
{
    for(std::string user: usersWantToChat)
    {
        std::cout << user << std::endl;
    }
}
