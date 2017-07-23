#ifndef HELPER_H
#define HELPER_H

#include <asio.hpp>
#include <define.h>

typedef std::vector<ByteBufferPtr> BuffersVector;
typedef std::vector<asio::const_buffer> BufferSequence;

class Helper
{
public:
    static void addCodeCommand(CodeCommand code, ByteBufferPtr bufferPtr);
    static BuffersVector addBufferSize(ByteBufferPtr buffer);
    static BufferSequence toBufferSequence(BuffersVector buffers);
    static std::vector<std::string> splitCommandAndData(const std::string &s);
    static std::pair<CodeCommand, ByteBufferPtr> getCodeAndData(const std::string &str);
    static void eraseCodeCommand(ByteBufferPtr bufferPtr);
    static std::string bufferToString(ByteBufferPtr bufferPtr);
    static ByteBuffer stringToBuffer(const std::string &str);
};

#endif // HELPER_H
