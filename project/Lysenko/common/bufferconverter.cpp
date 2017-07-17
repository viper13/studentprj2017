#include "bufferconverter.h"



BuffersVector BufferConverter::addMessageSize(ByteBufferPtr sourceMessage)
{
    BuffersVector resultBuffer(0);

    ByteBufferPtr sizeBuffer(new ByteBuffer());
    sizeBuffer->resize(2);

    uint16_t bufferSize = sourceMessage->size();

    (*sizeBuffer)[0] = ( (bufferSize & 0xff00) >> 8 );
    (*sizeBuffer)[1] = ( bufferSize & 0x00ff );

    resultBuffer.push_back(sizeBuffer);
    resultBuffer.push_back(sourceMessage);

    return resultBuffer;
}



WriteBuffer BufferConverter::toWriteBuffer(BuffersVector sourceBuffer)
{
    WriteBuffer result(0);

    for (ByteBufferPtr buffer : sourceBuffer)
    {
        result.push_back( asio::buffer (*buffer) );
    }

    return result;
}

WriteBuffer BufferConverter::getMessage(ByteBufferPtr sourceMessage)
{
    return BufferConverter::toWriteBuffer(
                BufferConverter::addMessageSize (sourceMessage) );
}



uint16_t BufferConverter::charsToMessageSize(ByteBuffer& sourceBuffer)
{
    uint16_t result = 0;

    for (uint i = 0; i < sourceBuffer.size(); ++i)
    {
        result += sourceBuffer[i] << 8 * (sourceBuffer.size() - 1 - i);
    }

    return result;
}
