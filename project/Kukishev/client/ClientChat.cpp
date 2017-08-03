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
        bool isLoginSuccessful = static_cast<bool>((*bufferPtr)[1]);
        if( isLoginSuccessful )
        {
            std::cout << "SYSTEM: Welcome!" << std::endl;
        }
        else
        {
            USER_INFO(Helper::bufferToString(bufferPtr, 1));
        }
        break;
    }
    case CommandCode::USER_LIST:
    {
        USER_INFO(Helper::bufferToString(bufferPtr, 1));
        break;
    }
    case CommandCode::CONNECT_TO_USER:
    {
        std::string userName = Helper::bufferToString(bufferPtr, 1);
        USER_INFO("SYSTEM: " + userName + " want star chat with you!" << std::endl
                  << "Use command of confirm to start chat");

        break;
    }
    case CommandCode::ANSWER_ON_REQUEST_TO_CONNECT:
    {
        std::string userName = Helper::bufferToString(bufferPtr, 1);

        USER_INFO("SYSTEM: User " + userName + " had confirmed your request to start chat");
        break;
    }
    case CommandCode::SEND_MESSAGE:
    {
        USER_INFO(Helper::bufferToString(bufferPtr, 1));
        break;
    }
    case CommandCode::SHOW_QUEUE_USERS:
    {
        USER_INFO(Helper::bufferToString(bufferPtr, 1));
        break;
    }
    case CommandCode::SHOW_CHATS:
    {
        USER_INFO(Helper::bufferToString(bufferPtr, 1));
        break;
    }
    default:
        break;
    }
}

void ClientChat::execute(CommandCode cmd, ByteBufferPtr bufferPtr)
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
        disconnectFromUser(bufferPtr);
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
    case CommandCode::ENTER_CHAT:
    {
        enterChat(bufferPtr);
        break;
    }
    case CommandCode::SHOW_CHATS:
    {
        showChats();
        break;
    }
    case CommandCode::OUT_FROM_CHAT:
    {
        outChat();
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
        USER_INFO("SYSTEM: Login is empty!");
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
        USER_INFO("SYSTEM: Message is empty!");
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
        USER_INFO("SYSTEM: User's name is empty!");
        return;
    }

    Helper::insertCommandCode(userName, CommandCode::CONNECT_TO_USER);
    write(userName);

}

void ClientChat::disconnectFromUser(ByteBufferPtr userName)
{
    if(userName->empty())
    {
        USER_INFO("SYSTEM: User's name is empty!");
        return;
    }

    Helper::insertCommandCode(userName, CommandCode::DISCONNECT_FROM_USER);
    write(userName);
}

void ClientChat::answerOnRequestToConnect(ByteBufferPtr userNameAndAnswer)
{
    Helper::insertCommandCode(userNameAndAnswer, CommandCode::ANSWER_ON_REQUEST_TO_CONNECT);
    write(userNameAndAnswer);
}

void ClientChat::confirmToStarChat(ByteBufferPtr userName)
{
    execute(CommandCode::ANSWER_ON_REQUEST_TO_CONNECT, std::move(userName));
}

void ClientChat::singUp(ByteBufferPtr userName)
{
    if(userName->empty())
    {
        USER_INFO("SYSTEM: User's name is empty!");
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

void ClientChat::showChats()
{
    ByteBufferPtr buff = std::make_shared<ByteBuffer>();
    Helper::insertCommandCode(buff, CommandCode::SHOW_CHATS);
    write(buff);
}

void ClientChat::enterChat(ByteBufferPtr chatName)
{
    Helper::insertCommandCode(chatName, CommandCode::ENTER_CHAT);
    write(chatName);
}

void ClientChat::outChat()
{
    ByteBufferPtr buff = std::make_shared<ByteBuffer>();
    Helper::insertCommandCode(buff, CommandCode::OUT_FROM_CHAT);
    write(buff);
}
