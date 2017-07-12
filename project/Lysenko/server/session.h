#ifndef SESSION_H
#define SESSION_H

#include <memory>
#include <functional>

#include <asio.hpp>

#include "define.h"

class Session;

typedef std::shared_ptr<Session> SessionPtr;

class Session
        : public std::enable_shared_from_this<Session>
{
    public:

        Session();



        static SessionPtr getNewSession();



        void start();



        asio::ip::tcp::socket& getSocket();



    private:

        void handle_read(asio::error_code error, size_t bufferSize);



        ByteBuffer buffer_;
        asio::ip::tcp::socket socket_;
};

#endif // SESSION_H
