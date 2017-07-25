#ifndef SESSION_H
#define SESSION_H

#include <asio.hpp>

#include "define.h"
#include "BufferConverter.h"

class MessageSession;

class MessageSession
        : public std::enable_shared_from_this<MessageSession>
{
    public:

        MessageSession();

        void start();
        void write(std::string message);

        asio::ip::tcp::socket& getSocket();

    protected:

        virtual void onRead(const ByteBuffer& buffer) = 0;

        asio::ip::tcp::socket socket_;

    private:

        void readMessageSize();
        void readMessage(uint16_t messageSize);
        void handleWrite(BuffersVector data,
                         asio::error_code error,
                         size_t writtenBytesCount);
        void handleReadMsgSize(asio::error_code error, size_t bufferSize);
        void handleReadMessage(asio::error_code error, size_t bufferSize);

        ByteBuffer buffer_;
};

#endif // SESSION_H
