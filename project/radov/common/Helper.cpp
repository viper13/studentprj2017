#include "Helper.h"

BuffersVector Helper::addSize(ByteBufferPtr buffer)
{
    BuffersVector result;
    ByteBufferPtr size_buffer(new ByteBuffer());
    size_buffer -> resize(2);
    uint16_t size = buffer -> size();
    (*size_buffer)[0] = ((size & 0xff00) >> 8);
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

uint16_t Helper::getSize(uint8_t left, uint8_t right)
{
    return ((left << 8) + right);
}


bool Helper::parseChatMessages(const pqxx::tuple &data, ChatMessage &chatMessage)
{
    chatMessage.id_ = data["id"].as<int>();
    chatMessage.chat_id_ = data["chat_id"].as<std::string>();
    chatMessage.user_id_ = data["user_id"].as<std::string>();
    chatMessage.message_ = data["message"].as<std::string>();

    return true;
}

void Helper::prependCommand(Commands command, std::string &message)
{
    message.erase();
    message.insert(message.begin(), (char)command);
}
