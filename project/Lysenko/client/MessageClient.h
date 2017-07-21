#ifndef CLIENT_H
#define CLIENT_H

#include "BufferConverter.h"

class MessageClient
        : public std::enable_shared_from_this<MessageClient>
{
    public:

        MessageClient(std::string address, std::string port);

        void start();

        virtual ~MessageClient() = default;

    protected:

        void write(std::string message);

        asio::io_service& io_service_;
        asio::ip::tcp::socket socket_;

        std::string address_;
        std::string port_;

        asio::ip::tcp::resolver resolver_;

        ByteBuffer buffer_;

    private:

        void handleResolveEndPoint(asio::error_code error,
                                   asio::ip::tcp::resolver::iterator iterator);
        void handleConnect(asio::error_code error,
                                   asio::ip::tcp::resolver::iterator iterator);
        void readMessageSize();
        void readMessage(uint16_t messageSize);
        void handleReadMsgSize(asio::error_code error, size_t bufferSize);
        void handleReadMessage(asio::error_code error, size_t bufferSize);
        void handleWrite(BuffersVector data,
                         asio::error_code error,
                         size_t writtenBytesCount);
};

#endif // CLIENT_H
