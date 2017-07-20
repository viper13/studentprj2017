#include "Helper.h"


ByteBufferPtr Helper::makeBuffer(ByteBufferPtr buffPtr)
{
    uint16_t msgSize = buffPtr->size();

    uint16_t lowByte = msgSize & 0x00FF;
    uint16_t highByte = msgSize >> 8;

//    ByteBufferPtr buffer(new ByteBuffer());
//    buffer->resize(2);
    buffPtr->emplace(buffPtr->begin(), static_cast<char>(lowByte));
    buffPtr->emplace(buffPtr->begin(), static_cast<char>(highByte));
//    (*buffer)[0] = static_cast<char>(highByte);
//    (*buffer)[1] = static_cast<char>(lowByte);


//    for(char sym : message)
//        buffer->emplace_back(sym);

    return buffPtr;
}

uint16_t Helper::mergeTwoByte(uint8_t high, uint8_t low)
{
    return (high << 8) | low;
}

void Helper::insertCommandCode(ByteBufferPtr buffPtr, CommandCode code)
{
    buffPtr->emplace(buffPtr->begin(), static_cast<uint8_t>(code));

}

ByteBuffer Helper::stringToBuffer(const std::string &str)
{
    return ByteBuffer(str.begin(), str.end());
}

std::string Helper::bufferToString(ByteBufferPtr buffPtr, uint posFrom, uint posTo)
{
    return std::string(buffPtr->begin()+posFrom, buffPtr->end() - posTo);
}

