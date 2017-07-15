#ifndef HELPER_H
#define HELPER_H

#include <asio.hpp>
#include <define.h>
#include <helper.h>

typedef std::vector<ByteBufferPtr> BuffersVector;
typedef std::vector<asio::const_buffer> BufferSequance;
class Helper
{
public:
    static BuffersVector addSizeValue(ByteBufferPtr buffer);
    static BufferSequance toBufferSequance(BuffersVector buffers);
    Helper();
};

#endif // HELPER_H
