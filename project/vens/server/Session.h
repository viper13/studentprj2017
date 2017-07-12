#ifndef SESSION_H
#define SESSION_H

#include <memory>
#include <asio.hpp>
#include <define.h>

class Session
        : public std::enable_shared_from_this<Session>
{    
public:
    Session();

    static std::shared_ptr<Session> getNewSession();

    void start();

    asio::ip::tcp::socket& socket();

    void write(std::string message);

private:
    void read();

    void handleRead(asio::error_code error, size_t bufferSize);

    void handleWrite(ByteBufferPtr data
                     , asio::error_code error
                     , size_t writedBytesCount);

    asio::ip::tcp::socket socket_;
    ByteBuffer buffer_;
};

typedef std::shared_ptr<Session> SessionPtr;

#endif // SESSION_H
