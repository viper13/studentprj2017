#ifndef HELPER_H
#define HELPER_H

#include <asio.hpp>
#include <pqxx/pqxx>
#include <define.h>

typedef std::vector<asio::const_buffer> BufferSequence;

class Helper
{
public:
    static BuffersVector addSizeBuffer(ByteBufferPtr buffer);
    static BufferSequence toBufferSequence(BuffersVector buffers);
    static bool paceFromPostgres(const pqxx::tuple& data, User& user);
};

#endif // HELPER_H
