#ifndef SESSION_H
#define SESSION_H

#include <memory>
#include <boost/asio.hpp>
#include <define.h>

using namespace boost;

class Session
        : public std::enable_shared_from_this<Session>
{
public:
    Session();

    static std::shared_ptr<Session> getNewSession();

    void start();

    asio::ip::tcp::socket& socket();

private:
    void handle_read(system::error_code error, size_t bufferSize);

    asio::ip::tcp::socket socket_;
    std::vector<char> buffer_;
};

typedef std::shared_ptr<Session> SessionPtr;

#endif // SESSION_H
