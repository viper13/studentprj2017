#include "define.h"

std::ostream &operator<<(std::ostream &stream, const ByteBuffer &matrix)
{
    std::string str(matrix.begin(), matrix.end());
    stream << "[ " << str << " ]";
    return stream;
}
