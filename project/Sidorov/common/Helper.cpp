#include "Helper.h"

Helper::Helper()
{

}

BuffersVector Helper::addBufferSize(ByteBufferPtr buffer)
{
    BuffersVector result;

    ByteBufferPtr bufferSize_(new ByteBuffer());
    bufferSize_->resize(2);
    int size = buffer->size();

    (*bufferSize_)[0] = ((size & 0xff00) << 8);
    (*bufferSize_)[1] = (size & 0x00ff);
    result.push_back(bufferSize_);
    result.push_back(buffer);

    return result;
}

BufferSequence Helper::toBufferSequence(BuffersVector buffers)
{
    BufferSequence result;    
    for ( ByteBufferPtr buffer : buffers)
    {
        result.push_back(asio::buffer(*buffer));
    }
    return result;
}
