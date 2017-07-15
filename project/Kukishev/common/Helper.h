#ifndef HELPER_H
#define HELPER_H

#include "define.h"

class Helper
{
public:
    Helper() = delete;
    static ByteBufferPtr makeBuffer(const std::string& message);
    static uint16_t mergeTwoByte(uint8_t high, uint8_t low);
};

#endif // HELPER_H
