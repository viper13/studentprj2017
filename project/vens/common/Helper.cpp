#include "Helper.h"


BuffersVector Helper::addSizeBuffer(ByteBufferPtr buffer)
{
    BuffersVector result;

    ByteBufferPtr size_buffer(new ByteBuffer());
    size_buffer->resize(2);
    *(reinterpret_cast<uint16_t*>(size_buffer->data())) = static_cast<uint16_t>(buffer->size());
    result.push_back(size_buffer);
    result.push_back(buffer);

    return result;
}

BufferSequence Helper::toBufferSequence(BuffersVector buffers)
{
    BufferSequence result;
    for (ByteBufferPtr buffer : buffers)
    {
        result.push_back(asio::buffer(*buffer));
    }
    return result;
}
