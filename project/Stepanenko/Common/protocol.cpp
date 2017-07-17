#include "protocol.h"

Protocol::Protocol()
{

}

std::string Protocol::userListClientMessageCreate()
{
    char firstSymbol = messageType.USER_LIST;
    std::string result = "";
    result.push_back(firstSymbol);
    return result;
}


// It is good idea to use space symbol for dividing users into our message,
// so reading will be much more simpler.
// Of course, names should not contain spaces inside themselfs.

std::string Protocol::userListServerMessageCreate(StringBufferPtr names)
{
    char firstSymbol = messageType.USER_LIST;
    std::string result = "";
    result.push_back(firstSymbol);

    for (string name : *names)
    {
        result.push_back(name);
        result.push_back(" ");
    }
    return result.substr(0, result.length()-1);
}

StringSetPtr Protocol::userListServerMessageParse(std::string message)
{
    StringSetPtr result;
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
    char firstSymbol = messageType.MESSAGE;
    std::stringstream tempStream;
    tempStream << firstSymbol << user << ": " << message;
    return tempStream.str();
}

std::string Protocol::chatMessageClientMessageParse(std::string message)
{
    return message.substr(1, message.length());
}

