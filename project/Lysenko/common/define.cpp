#include "define.h"
#include "BufferConverter.h"



std::ostream& operator<<(std::ostream& stream, const ByteBuffer& buffer)
{
    std::string str( buffer.begin(), buffer.end() );

    stream <<  "[" << str << "]";

    return stream;
}



std::ostream& operator<<(std::ostream& stream, const BuffersVector& buffersVector)
{
    for (ByteBufferPtr buffer : buffersVector)
    {
        stream << *buffer << ' ';
    }

    return stream;
}



std::ostream& operator<<(std::ostream& stream, const User& user)
{
    stream << "{ id = " << user.id_ <<
              " name = " << user.name_ << " }";

    return stream;
}
