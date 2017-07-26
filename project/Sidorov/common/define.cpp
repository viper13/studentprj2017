#include "define.h"

std::ostream& operator <<(std::ostream& stream, const ByteBuffer& buffer)
{
    std::string str (buffer.begin(), buffer.end());
    stream << "[" << str << "]";
    return stream;
}

std::ostream& operator <<(std::ostream& stream, const User& user)
{
    stream << "User{ id: " << user.id_
           << " name: [" << user.name_
           << "] nick: [" << user.nick_ << "]}";
    return stream;
}
