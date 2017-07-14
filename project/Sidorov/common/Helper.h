#ifndef HELPER_H
#define HELPER_H

#include <asio.hpp>
#include <define.h>

typedef std::vector<ByteBufferPtr> BuffersVector;
typedef std::vector<asio::const_buffer> BufferSequence;

class Helper
{
public:
    static BuffersVector addSizeBuffer(ByteBufferPtr buffer);
    static BufferSequence toBufferSequence(BuffersVector buffers);
};

#endif // HELPER_H
