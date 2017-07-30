#include "protocol.h"

Protocol::Protocol()
{

}

std::string Protocol::typeRemover(std::string message)
{
    return message.substr(1, message.length());
}

std::string Protocol::userListClientMessageCreate()
{
    char firstSymbol = Type::USER_LIST;
    std::string result = "";
    result.push_back(firstSymbol);
    return result;
}


// It is good idea to use space symbol for dividing users into our message,
// so reading will be much more simpler.
// Of course, names should not contain spaces inside themselfs.

std::string Protocol::userListServerMessageCreate(std::set<std::string> names)
{
    char firstSymbol = Type::USER_LIST;
    std::string result = "";
    result.push_back(firstSymbol);

    for (std::string name : names)
    {
        result += name;
        result += " ";
    }
    return result;
}

StringSetPtr Protocol::userListServerMessageParse(std::string message)
{
    StringSetPtr result = StringSetPtr(new std::set<std::string>());
    std::stringstream tempStream(message.substr(1, message.length()));
    std::string tempString;
    while(tempStream >> tempString)
    {
        result->insert(tempString);
    }
    return result;
}



std::string Protocol::chatMessageClientMessageCreate(std::string user, std::string message)
{
    char firstSymbol = Type::MESSAGE;
    std::stringstream tempStream;
    tempStream << firstSymbol << user << ": " << message;
    return tempStream.str();
}

std::string Protocol::chatMessageClientMessageParse(std::string message)
{
    return message.substr(1, message.length());
}

std::string Protocol::logInClientMessageCreate(std::string name)
{
    return typeAdder(Type::LOG_IN, name);
}

std::string Protocol::logInServerMessageCreate(Status status)
{
    return typeAdder(Type::LOG_IN, status);
}

std::string Protocol::startChatClientMessageCreate(std::string name)
{
    return typeAdder(Type::START_CHAT, name);
}

std::string Protocol::startChatServerMessageCreate(Status status)
{
    return typeAdder(Type::START_CHAT, status);
}

std::string Protocol::disconnectClientMessageCreate()
{
    return typeAdder(Type::USER_DISCONNECT, "");
}

std::string Protocol::disconnectServerMessageCreate(Status status)
{
    return typeAdder(Type::USER_DISCONNECT, status);
}

std::string Protocol::createChatClientMessageCreate(const std::string &nameOfChat)
{
    return typeAdder(Type::CREATE_CHAT, nameOfChat);
}

std::string Protocol::createChatServerMessageCreate(Protocol::Status status)
{
    return typeAdder(Type::CREATE_CHAT, status);
}

std::string Protocol::chatListClientMessageCreate()
{
    return typeAdder(Type::CHAT_LIST, "");
}

std::string Protocol::chatListServerMessageCreate(const std::set<std::string> &chatNames)
{
    std::string result = "";
    result.push_back(Type::CHAT_LIST);

    for (std::string name : chatNames)
    {
        result += name;
        result += " ";
    }
    return result;
}

void Protocol::chatListServerMessageParse(std::string message, std::set<std::string> &chatNames)
{
    std::stringstream tempStream(message.substr(1, message.length()));
    std::string tempString;
    while(tempStream >> tempString)
    {
        chatNames.insert(tempString);
    }
}

std::string Protocol::joinChatClientMessageCreate(const std::string &nameOfChat)
{
    return typeAdder(Type::JOIN_CHAT, nameOfChat);
}

std::string Protocol::joinChatServerMessageCreate(Protocol::Status status)
{
    return typeAdder(Type::JOIN_CHAT, status);
}

std::string Protocol::typeAdder(Type type, std::string message)
{

    std::string result = "";
    result.push_back(type);
    result += message;
    return result;
}

std::string Protocol::typeAdder(Protocol::Type type, Status status)
{
    std::string result = "";
    result.push_back(type);
    result.push_back(status);
    return result;
}

