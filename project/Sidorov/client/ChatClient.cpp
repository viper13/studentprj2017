#include "ChatClient.h"

ChatClient::ChatClient(std::string address, std::string port)
    : Client(address,port)
{
    username_ = "";
    inChatWith_="";
}


void ChatClient::onRead(ByteBufferPtr data)
{
    CodeCommand code = static_cast<CodeCommand>(data->at(0));
    Helper::eraseCodeCommand(data);
    switch (code)
    {
    case CodeCommand::CONNECT_TO_USER:
    {
        std::string userName = Helper::bufferToString(data);
        std::cout << '\n' << userName << " wants to chat with you" << "\n"
                 << "Use command ACCEPT_TO_CHAT [name]" << "\n" ;
        break;
    }
    case CodeCommand::SEE_REQUESTS:
    {
        std::string names = Helper::bufferToString(data);
        std::cout << "\nYour requests: " << names  << '\n' << std::endl;
        break;
    }
    case CodeCommand::SEE_FRIENDS:
    {
        std::string names = Helper::bufferToString(data);
        std::cout << "\nYour friends: " << names << '\n'<< std::endl;
        break;
    }
    case CodeCommand::START_CHAT:
    {
        std::string partner = Helper::bufferToString(data);
        LOG_INFO("\nYour current chatman - " << partner);
        break;
    }
    default:
        std::string message = Helper::bufferToString(data);
        std::cout << '\n' <<message << '\n'<< std::endl;
        break;
    }
}

void ChatClient::execute(CodeCommand code, ByteBufferPtr bufferPtr)
{
    switch (code)
    {
    case CodeCommand::CONNECT_TO_USER:
    {
        connectToUser(bufferPtr);
        break;
    }
    case CodeCommand::DISCONNECT_FROM_USER:
    {
        disconnectFromUser();
        break;
    }
    case CodeCommand::REGISTRATION:
    {
        registration(bufferPtr);
        break;
    }
    case CodeCommand::LOGIN:
    {
        login(bufferPtr);
        break;
    }
    case CodeCommand::LOGOUT:
    {
        logout();
        break;
    }
    case CodeCommand::SEND_MESSAGE:
    {
        sendMessage(bufferPtr);
        break;
    }
    case CodeCommand::USER_LIST:
    {
        getUserList();
        break;
    }
    case CodeCommand::SEE_REQUESTS:
    {
        seeRequests();
        break;
    }
    case CodeCommand::SEE_FRIENDS:
    {
        seeFriends();
        break;
    }
    case CodeCommand::ACCEPT_TO_CHAT:
    {
        acceptToChat(bufferPtr);
        break;
    }
    case CodeCommand::PRINT_HELP:
    {
        printHelp();
        break;
    }
    case CodeCommand::START_CHAT:
    {
        startChat(bufferPtr);
        break;
    }
    default:
        break;
    }
}


void ChatClient::registration(ByteBufferPtr name)
{
    if ( 0 == name->size())
    {
        LOG_INFO("Input your UserName");
        return;
    }
    Helper::addCodeCommand(CodeCommand::REGISTRATION,name);
    write(name);
}

void ChatClient::login(ByteBufferPtr name)
{
    if ( 0 == name->size())
    {
        LOG_INFO("Input your Login");
        return;
    }
    Helper::addCodeCommand(CodeCommand::LOGIN,name);
    write(name);
}

void ChatClient::logout()
{
    ByteBufferPtr bufferPtr = std::make_shared<ByteBuffer>();
    Helper::addCodeCommand(CodeCommand::LOGOUT,bufferPtr);
    write(bufferPtr);
}

void ChatClient::sendMessage(ByteBufferPtr message)
{
    if ( 0 == message->size())
    {
        LOG_INFO("Input your Message");
        return;
    }
    Helper::addCodeCommand(CodeCommand::SEND_MESSAGE, message);
    write(message);
}

void ChatClient::getUserList()
{
    ByteBufferPtr bufferPtr = std::make_shared<ByteBuffer>();
    Helper::addCodeCommand(CodeCommand::USER_LIST,bufferPtr);
    write(bufferPtr);
}

void ChatClient::connectToUser(ByteBufferPtr userName)
{
    if ( 0 == userName->size())
    {
        LOG_ERR("Input UserName");
        return;
    }
    Helper::addCodeCommand(CodeCommand::CONNECT_TO_USER, userName);
    write(userName);
}

void ChatClient::disconnectFromUser()
{
    ByteBufferPtr bufferPtr = std::make_shared<ByteBuffer>();
    Helper::addCodeCommand(CodeCommand::DISCONNECT_FROM_USER,bufferPtr);
    write(bufferPtr);
}

void ChatClient::acceptToChat(ByteBufferPtr userName)
{
    Helper::addCodeCommand(CodeCommand::ACCEPT_TO_CHAT, userName);
    write(userName);
}

void ChatClient::seeRequests()
{
    ByteBufferPtr bufferPtr = std::make_shared<ByteBuffer>();
    Helper::addCodeCommand(CodeCommand::SEE_REQUESTS,bufferPtr);
    write(bufferPtr);
}

void ChatClient::seeFriends()
{
    ByteBufferPtr bufferPtr = std::make_shared<ByteBuffer>();
    Helper::addCodeCommand(CodeCommand::SEE_FRIENDS,bufferPtr);
    write(bufferPtr);
}

void ChatClient::printHelp()
{
    std::cout << "Use next numbers for command:" << std::endl
              << " '1 [name]' - (REGISTRATION)      --  Register your in our base" << std::endl
              << " '2 [name]' - (LOGIN)             --  Authorization (use this command after registration)" << std::endl
              << " '3'        - (LOGOUT)            " << std::endl
              << " '4 [name]' - (CONNECT_TO_USER)   --  Send request to chat to user" << std::endl
              << " '5 [name]' - (ACCEPT_TO_CHAT)    --  Accept request to chat (use '9' to see your requests)" << std::endl
              << " '6 [name]' - (START_CHAT)        --  Start chat with user which accept your request to chat" << std::endl
              << " '7 [text]' - (SEND_MESSAGE)      --  Send message to chat user" << std::endl
              << " '8'        - (DISCONNECT_FROM_USER) -- Disconnect from your current chat user" << std::endl
              << " '9'        - (SEE_REQUESTS)      -- Show requests from other users" << std::endl
              << " '10'       - (SEE_FRIENDS)       -- Show your friend list" << std::endl
              << " '11'       - (USER_LIST)         --  Output list of registered users (online,offline)" << std::endl
              << " '12'       - (PRINT HELP)        -- Output command list" << std::endl
              << " '13'       - (EXIT)" << std::endl;
}

void ChatClient::startChat(ByteBufferPtr userName)
{
    if ( 0 == userName->size())
    {
        LOG_ERR("Input your partner name");
        return;
    }
    Helper::addCodeCommand(CodeCommand::START_CHAT, userName);
    write(userName);
}
