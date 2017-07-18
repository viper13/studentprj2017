#ifndef HELPER_H
#define HELPER_H

#include "define.h"

class Helper
{
public:
    Helper() = delete;
    static ByteBufferPtr makeBuffer(ByteBufferPtr message);
    static uint16_t mergeTwoByte(uint8_t high, uint8_t low);
    static void insertCommandCode(ByteBufferPtr buffPtr, CommandCode code);
};

#endif // HELPER_H
