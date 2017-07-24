#include "define.h"

std::ostream &operator<<(std::ostream &stream, const ByteBuffer &matrix)
{
    std::string str(matrix.begin(), matrix.end());
    stream << "[ " << str << " ]";
    return stream;
}

std::ostream &operator<<(std::ostream &stream, const NewUser &user)
{
    stream << "User {id: " << user.id
           << " name: [" << user.name
           <<"] nick: [" << user.nick << "]}";
    return stream;
}
