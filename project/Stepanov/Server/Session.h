#ifndef SESSION_H
#define SESSION_H

#include <memory>
#include <boost/asio.hpp>

#include "define.h"

class Session
        : public std::enable_shared_from_this<Session>
{

public:
    Session(boost::asio::ip::tcp::socket &socket);

    static std::shared_ptr<Session> getNewSession(boost::asio::ip::tcp::socket&);

    void start();

private:
    boost::asio::ip::tcp::socket& socket_;
    std::vector<std::string> buffer_;

    void handle_read(boost::system::error_code error, size_t bufferSize);
};

typedef std::shared_ptr<Session> SessionPtr;

#endif // SESSION_H
