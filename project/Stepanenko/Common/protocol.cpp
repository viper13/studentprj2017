#include "protocol.h"

Protocol::Protocol()
{

}

std::string Protocol::userListClientMessageCreate()
{
    char firstSymbol = USER_LIST;
    std::string result = "";
    result.push_back(firstSymbol);
    return result;
}


// It is good idea to use space symbol for dividing users into our message,
// so reading will be much more simpler.
// Of course, names should not contain spaces inside themselfs.

std::string Protocol::userListServerMessageCreate(StringBufferPtr names)
{
    char firstSymbol = USER_LIST;
    std::string result = "";
    result.push_back(firstSymbol);

    for (string name : *names)
    {
        result.push_back(name);
        result.push_back(" ");
    }
    return result.substr(0, result.length()-1);
}

StringBufferPtr Protocol::userListServerMessageParse(std::string message)
{
    StringBufferPtr result;
    std::stringstream tempStream(message.substr(1, message.length()));
    std::string tempString;
    while(tempStream >> tempString)
    {
        result->push_back(tempString);
    }
    return result;
}



std::string Protocol::chatMessageClientMessage(std::string user, std::string message)
{
    char firstSymbol = MESSAGE;
    std::stringstream tempStream;
    tempStream << firstSymbol << user << ": " << message;
    return tempStream.str();
}

