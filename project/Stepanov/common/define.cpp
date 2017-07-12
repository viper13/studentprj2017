#include "Define.h"
std::ostream& operator<<(std::ostream& stream,const ByteBuffer& buffer)
{
    std::string str(buffer.begin(),buffer.end());
    stream<< str;
    return stream;
}
