#ifndef SESION_H
#define SESION_H

#include <define.h>
#include <helper.h>
#include <memory>
#include <asio.hpp>

class Session
        : public std::enable_shared_from_this<Session>
{
public:
    Session(int id);
    static std::shared_ptr<Session> getNewSession(int id);
    void start();
    asio::ip::tcp::socket &socket();
    void write(std::string message);
    std::string getUserName();
private:
    void read();

    void handleRead(asio::error_code error, size_t bufferSize);
    void handleWrite(BuffersVector data, asio::error_code error, size_t bufferSize);
    void setUserName(std::string &userName);
    int id_;
    std::string userName_;
    asio::ip::tcp::socket socket_;
    std::vector<char> buffer_;
    uint16_t nextMessageSize_;
};

typedef std::shared_ptr<Session> SessionPtr;

#endif // SESION_H
