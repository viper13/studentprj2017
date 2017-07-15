#include "helper.h"

BuffersVector Helper::addSizeValue(ByteBufferPtr buffer)
{
    BuffersVector result;
    ByteBufferPtr size_buffer(new ByteBuffer());
    size_buffer->resize(2);
    uint16_t size = buffer->size();
    (*size_buffer)[0] = ((size & 0xff00) << 8);
    (*size_buffer)[1] = (size & 0x00ff);
    LOG_INFO("[" << (int)(*size_buffer)[0] << "][" << (int)(*size_buffer)[1] << "]");
    result.push_back(buffer);
    return result;
}

BufferSequance Helper::toBufferSequance(BuffersVector buffers)
{
    BufferSequance result;
    for (ByteBufferPtr buffer : buffers)
    {
        result.push_back(asio::buffer(*buffer));
    }
    return result;
}

Helper::Helper()
{

}

