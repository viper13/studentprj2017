#ifndef HELPER_H
#define HELPER_H

#include <asio.hpp>
#include <define.h>
#include <helper.h>
#include <pqxx/pqxx>

typedef std::vector<ByteBufferPtr> BuffersVector;
typedef std::vector<asio::const_buffer> BufferSequance;
class Helper
{
public:
    static BuffersVector addSizeValue(ByteBufferPtr buffer);
    static BufferSequance toBufferSequance(BuffersVector buffers);
    static bool paceFromPostgres(const pqxx::tuple &data, User& user);
    static std::string getChatRoomName(const std::string &user1, const std::string &user2);
    Helper();
};

#endif // HELPER_H
