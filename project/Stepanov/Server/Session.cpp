#include "Session.h"
#include <functional>

Session::Session(boost::asio::ip::tcp::socket &socket)
    : socket_(socket)
{
    buffer_.resize(BUFFER_MAX_SIZE);

}

std::shared_ptr<Session> Session::getNewSession(boost::asio::ip::tcp::socket &socket)
{
    SessionPtr session(new Session(socket));

    return session;
}

void Session::start()
{
    boost::asio::async_read(socket_
                            , boost::asio::buffer(buffer_.c_str(),BUFFER_MAX_SIZE)
                            , std::bind(&Session::handle_read
                                        , shared_from_this()
                                        , std::placeholder::_1
                                        , std::placeholder::_2

                                        ));
}

void Session::handle_read(boost::system::error_code error, size_t bufferSize)
{
    if(!error)
    {
        buffer_.resize(bufferSize);
        LOG_INFO("Message: []\n");

        start();
    }
    else
    {
        LOG_ERR("Failure: error code"<<error.value()<<" description " << error.message());
    }
}
