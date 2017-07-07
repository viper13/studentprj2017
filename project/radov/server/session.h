#ifndef SESSION_H
#define SESSION_H

#include <memory>
#include <asio.hpp>

class Session
        : public std::enable_shared_from_this<Session>
{

    public:

        Session(asio::ip::tcp::socket& socket);

        std::shared_ptr<Session> getNewSession();

        void start();

    private:
        void handle_read(asio::error_code error, size_t bufferSize);

        asio::ip::tcp::socket& socket;
        std::string buffer;
};

typedef std::shared_ptr<Session> SessionPtr;



#endif // SESSION_H
