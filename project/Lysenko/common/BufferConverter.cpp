#include "BufferConverter.h"



BuffersVector BufferConverter::addMessageSize(ByteBufferPtr sourceMessage)
{
    BuffersVector resultBuffer(0);

    ByteBufferPtr sizeBuffer =
                std::make_shared<ByteBuffer> (
                    BufferConverter::uint16ToBuffer( sourceMessage->size() )
                                             );

    resultBuffer.push_back(sizeBuffer);
    resultBuffer.push_back(sourceMessage);

    return resultBuffer;
}



std::string BufferConverter::addOpCode(Operation op, std::string sourceString)
{
    sourceString.insert( sourceString.begin(), static_cast<char>(op));

    return sourceString;
}



ByteBuffer BufferConverter::uint16ToBuffer(uint16_t number)
{
    ByteBuffer result(2);
    result[0] = ( (number & 0xff00) >> 8 );
    result[1] = ( number & 0x00ff );

    return result;
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



uint16_t BufferConverter::bufferToUint16(ByteBuffer& sourceBuffer)
{
    uint16_t result = (sourceBuffer[0] << 8) + sourceBuffer[1];

    /*for (uint i = 0; i < sourceBuffer.size(); ++i)
    {
        result += sourceBuffer[i] << 8 * (sourceBuffer.size() - 1 - i);
    }*/

    return result;
}
