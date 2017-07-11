#ifndef SESSION_H
#define SESSION_H

#include <memory>
#include <boost/asio.hpp>
#include <Define.h>

using namespace boost;

class Session
        : public std::enable_shared_from_this<Session>
{
public:
    Session();

    asio::ip::tcp::socket& socket();

    void start();

    static std::shared_ptr<Session> getNewSession();

private:
    asio::ip::tcp::socket socket_;

    std::vector<char> buffer_;

    void handle_read(system::error_code error, size_t bufferSize);
};

typedef std::shared_ptr<Session> SessionPtr;

#endif // SESSION_H
