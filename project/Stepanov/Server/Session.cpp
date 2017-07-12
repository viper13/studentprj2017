#include "Session.h"
#include "Worker.h"

using namespace boost;

Session::Session()
    : socket_(Worker::instance()->io_service())
{

}

std::shared_ptr<Session> Session::getNewSession()
{
    std::shared_ptr<Session> result(new Session());

    return result;
}

void Session::write(std::string message)
{
    ByteBufferPtr buffer(new ByteBuffer(message.begin(), message.end()));

    asio::async_write(  socket_
                      , asio::buffer(*buffer)
                      , std::bind(&Session::handleWrite
                                  , shared_from_this()
                                  , buffer
                                  , std::placeholders::_1
                                  , std::placeholders::_2));
}



void Session::start()
{
    read();
}

asio::ip::tcp::socket &Session::socket()
{
    return socket_;
}

void Session::handle_read(system::error_code error, size_t bufferSize)
{
    if (!error)
    {

        buffer_.resize(bufferSize);

        LOG_INFO("Message: " << buffer_);
        std::string message(buffer_.begin(), buffer_.end());

        write(message);

        read();
    }
    else
    {
        LOG_ERR("Failure: read error code " << error.value()
                << " description: " << error.message()<< std::endl);
    }
}

void Session::handleWrite(ByteBufferPtr data,system::error_code error,size_t writedSize)
{
    if(!error)
    {
        LOG_INFO("Data writed successful! size ="
                 << data->size()
                 << "writed size="
                 << writedSize);
    }
    else
    {
        LOG_ERR("Failure to write data" << *data);
    }
}

void Session::read()
{
    buffer_.resize(BUFFER_MAX_SIZE);
    asio::async_read(  socket_
                     , asio::buffer(buffer_, BUFFER_MAX_SIZE)
                     , asio::transfer_at_least(1)
                     , std::bind(&Session::handle_read
                                 , shared_from_this()
                                 , std::placeholders::_1
                                 , std::placeholders::_2));


}
