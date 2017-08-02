#ifndef SESION_H
#define SESION_H

#include <define.h>
#include <Helper.h>
#include <memory>
#include <asio.hpp>

class Session
        : public std::enable_shared_from_this<Session>
{
public:
    Session();
    ~Session();
    void start();
    asio::ip::tcp::socket &socket();
    void write(std::string message);
    virtual void onRead(ByteBuffer data) = 0;

    std::string idClient() const;
    void setIdClient(const std::string &idClient);

    std::string idTarget() const;
    void setIdTarget(const std::string &idTarget);



    std::string clientPassword() const;
    void setClientPassword(const std::string &clientPassword);

private:
    void read();
    void handleRead(asio::error_code error, size_t bufferSize);
    void handleWrite(BuffersVector data, asio::error_code error, size_t bufferSize);

    asio::ip::tcp::socket socket_;
    uint16_t messageSize_;

    std::string idClient_;
    std::string idTarget_;
    std::string clientPassword_;

protected:
    ByteBuffer buffer_;

};

typedef std::shared_ptr<Session> SessionPtr;

#endif // SESION_H
