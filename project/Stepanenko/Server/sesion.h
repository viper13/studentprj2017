#ifndef SESION_H
#define SESION_H

#include <define.h>
#include <memory>
#include <asio.hpp>

class Session
        : public std::enable_shared_from_this<Session>
{
public:
    Session();
    static std::shared_ptr<Session> getNewSession();

    void start();

    asio::ip::tcp::socket &socket();

private:
    void handle_read(asio::error_code error, size_t bufferSize);

    asio::ip::tcp::socket socket_;
    std::vector<char> buffer_;
};

typedef std::shared_ptr<Session> SessionPtr;

#endif // SESION_H
