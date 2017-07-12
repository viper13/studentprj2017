#include "define.h"

std::ostream &operator<<(std::ostream &stream, const ByteBuffer &matrix)
{
    stream << "[ " <<  << " ]";
    return stream;
}
