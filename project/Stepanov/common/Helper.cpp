#include "Helper.h"

BuffersVector Helper::addSize(ByteBufferPtr buffer)
{
    BuffersVector result;
    ByteBufferPtr size_buffer(new ByteBuffer());
    size_buffer->resize(2);
    uint16_t size = buffer->size();
    (*size_buffer)[0] = ((size & 0xff00) << 8);
    (*size_buffer)[1] = (size & 0x00ff);
    result.push_back(size_buffer);
    result.push_back(buffer);
    return result;
}

EndBuffer Helper::makeEndBuffer(BuffersVector buffers)
{
    EndBuffer result;
    for (ByteBufferPtr buffer : buffers)
    {
        result.push_back(asio::buffer(*buffer));
    }
    return result;
}

uint16_t Helper::getSize(uint16_t left, uint16_t right)
{
    return ((left << 8) + right);
}

bool Helper::paceFromPostgres(const pqxx::tuple &data, User &user)
{
    user.id_=data["id"].as<int>();
    user.name=data["name"].as<std::string>();
    user.nick=data["nick"].as<std::string>();
    LOG_INFO(user);
    return true;
}

std::string Helper::makeRegisterMessage(std::string message)
{
    std::string result;
    result = LOGIN_MESSAGE+message;
    return result;
}

std::string Helper::makeListMessage()
{
    return GET_USER_LIST_MESSAGE;
}

std::string Helper::makeCreateChatMessage()
{
    std::string result;
    LOG_INFO("Enter id of target to create chat");
    std::cin >> result;
    result = CREATE_CHAT_MESSAGE + result;
    return result;
}

std::string Helper::makeDirectMessage()
{

}

std::string Helper::makeYesMessage(int currentRoom)
{
    std::string result;
    LOG_INFO("You accepted chat request!");
    result=YES_MESSAGE;
    result+=std::to_string(currentRoom);
    return result;

}

std::string Helper::makeHistoryMessage()
{
    return GET_CHAT_HISTORY;
}

std::string Helper::makeExitMessage()
{
    return EXIT_MESSAGE;
}

