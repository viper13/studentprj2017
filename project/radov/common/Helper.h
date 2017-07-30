#ifndef HELPER_H
#define HELPER_H

#include <define.h>
#include <pqxx/pqxx>
#include <Helper.h>

typedef std::vector<ByteBufferPtr> BuffersVector;
typedef std::vector<asio::const_buffer> EndBuffer;
class Helper
{
public:
    static BuffersVector addSize(ByteBufferPtr buffer);
    static EndBuffer makeEndBuffer(BuffersVector buffers);
    static uint16_t getSize(uint16_t left, uint16_t right);
    static bool parseFromPostgres(const pqxx::tuple& data, User& user);
    static void prependCommand(Commands command, std::string& message);
};

#endif // HELPER_H
