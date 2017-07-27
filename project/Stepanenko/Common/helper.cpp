#include "helper.h"

BuffersVector Helper::addSizeValue(ByteBufferPtr buffer)
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

BufferSequance Helper::toBufferSequance(BuffersVector buffers)
{
    BufferSequance result;
    for (ByteBufferPtr buffer : buffers)
    {
        result.push_back(asio::buffer(*buffer));
    }
    return result;
}

bool Helper::paceFromPostgres(const pqxx::tuple &data, User &user)
{
    user.id_ = data["id"].as<int>();
    user.name_ = data["name"].as<std::string>();
    user.nick_ = data["nick"].as<std::string>();
    return true;
}

std::string Helper::getChatRoomName(const std::string &user1, const std::string &user2)
{
    if (user1.compare(user2) > 0)
    {
        return (user1 + user2);
    }
    else
    {
        return (user2 + user1);
    }
}

Helper::Helper()
{

}

