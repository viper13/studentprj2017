#include "Define.h"
std::ostream& operator<<(std::ostream& stream,const ByteBuffer& buffer)
{
    std::string str(buffer.begin(),buffer.end());
    stream<< str;
    return stream;
}
std::ostream& operator<<(std::ostream& stream,const User& user)
{
    stream << "id: "<<user.id_<<std::endl
           << "name: "<<user.name<<std::endl
           << "nick: "<<user.nick<<std::endl;
    return stream;
}
