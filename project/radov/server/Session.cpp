#include "Session.h"
#include "Worker.h"
#include "Helper.h"

Session::Session()
    : socket_(Worker::instance() -> io_service())
    , messageSize_(0)
{

}

Session::~Session()
{
    if (socket_.is_open())
    {
        socket_.close();
        LOG_ERR("Socked Closed!!!");
    }
}



void Session::start()
{
    read();
}

asio::ip::tcp::socket &Session::socket()
{
    return socket_;
}

void Session::write(std::string message)
{
    ByteBufferPtr buffer(new ByteBuffer(message.begin(), message.end()));
    BuffersVector buffers = Helper::addSize(buffer);

    EndBuffer endBuffer = Helper::makeEndBuffer(buffers);

    asio::async_write(socket_
                      , endBuffer
                      , std::bind(&Session::handleWrite
                                  , shared_from_this()
                                  , buffers
                                  , std::placeholders::_1
                                  , std::placeholders::_2));
}

void Session::read()
{
    if (0 == messageSize_)
    {
        buffer_.resize(2);
        asio::async_read(socket_
                         , asio::buffer(buffer_, 2)
                         , asio::transfer_exactly(2)
                         , std::bind(&Session::handleRead
                                     , shared_from_this()
                                     , std::placeholders::_1
                                     , std::placeholders::_2));
    }
    else
    {
        buffer_.resize(messageSize_);
        asio::async_read(socket_
                         , asio::buffer(buffer_, messageSize_)
                         , asio::transfer_exactly(messageSize_)
                         , std::bind(&Session::handleRead
                                     , shared_from_this()
                                     , std::placeholders::_1
                                     , std::placeholders::_2));
    }

}

void Session::handleRead(asio::error_code error, size_t /*bufferSize*/)
{
    if (!error)
    {
        if (messageSize_ != 0)
        {
            messageSize_ = 0;
            onRead(buffer_);
            read();
        }
        else
        {
            messageSize_ = Helper::getSize(static_cast<uint16_t>(buffer_[0])
                                         , static_cast<uint16_t>(buffer_[1]));

            read();
        }
    }
    else
    {
        onUnexpectedClose();
        LOG_ERR("Failure: read error code " << error.value()
                 << " description: " << error.message());
    }
}

void Session::handleWrite(BuffersVector /*data*/, asio::error_code error, size_t /*bufferSize*/)
{
    if(!error)
    {

    }
    else
    {
        LOG_ERR("Failure write data." << error.message());
    }
}

std::string Session::clientPassword() const
{
    return clientPassword_;
}

void Session::setClientPassword(const std::string &clientPassword)
{
    clientPassword_ = clientPassword;
}

void Session::setIdClient(const std::string &idClient)
{
    idClient_ = idClient;
}

std::string Session::idTarget() const
{
    return idTarget_;
}

void Session::setIdTarget(const std::string &idTarget)
{
    idTarget_ = idTarget;
}

std::string Session::idClient() const
{
    return idClient_;
}