#include "Helper.h"


ByteBufferPtr Helper::makeBuffer(const std::string &message)
{
    uint16_t msgSize = message.size();

    uint16_t lowByte = msgSize & 0x00FF;
    uint16_t highByte = msgSize >> 8;

    ByteBufferPtr buffer(new ByteBuffer());
    buffer->resize(2);
    (*buffer)[0] = static_cast<char>(highByte);
    (*buffer)[1] = static_cast<char>(lowByte);


    for(char sym : message)
        buffer->emplace_back(sym);

    return buffer;
}

uint16_t Helper::mergeTwoByte(uint8_t high, uint8_t low)
{
    uint16_t l = static_cast<uint16_t>(low);
    uint16_t h = static_cast<uint16_t>(high);
    return (high << 8) | low;
}
