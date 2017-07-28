#include "Helper.h"

BuffersVector Helper::addSize(ByteBufferPtr buffer)
{
    BuffersVector result;
    ByteBufferPtr size_buffer(new ByteBuffer());
    size_buffer -> resize(2);
    uint16_t size = buffer -> size();
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

bool Helper::parseFromPostgres(const pqxx::tuple &data, User &user)
{
    user.id_ = data["id"].as<int>();
    user.name_ = data["name"].as<std::string>();
    user.nick_ = data["nick"].as<std::string>();

    return true;
}
