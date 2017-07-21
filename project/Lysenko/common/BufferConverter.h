#ifndef BUFFERCONVERTER_H
#define BUFFERCONVERTER_H

#include <vector>

#include <asio.hpp>

#include "define.h"

typedef std::vector<asio::const_buffer> WriteBuffer;

class BufferConverter
{
    public:

        static BuffersVector addMessageSize(ByteBufferPtr sourceMessage);
        static std::string addOpCode(Operation op, std::string sourceString);
        static WriteBuffer toWriteBuffer(BuffersVector sourceBuffer);
        static ByteBuffer uint16ToBuffer(uint16_t number);
        static uint16_t bufferToUint16(ByteBuffer& sourceBuffer);
};

#endif // BUFFERCONVERTER_H
