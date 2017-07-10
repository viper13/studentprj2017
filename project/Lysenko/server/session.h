#ifndef SESSION_H
#define SESSION_H

#include <memory>
#include <functional>
#include <asio.hpp>
#include <define.h>

class Session : public std::enable_shared_from_this<Session>
{
    public:
        Session();

        static std::shared_ptr<Session> getNewSession();

        void start();

        asio::ip::tcp::socket& socket();

    private:
        std::vector<char> buffer_;
        asio::ip::tcp::socket socket_;


        void handle_read(asio::error_code error, size_t bufferSize);
};

typedef std::shared_ptr<Session> SessionPtr;

#endif // SESSION_H
