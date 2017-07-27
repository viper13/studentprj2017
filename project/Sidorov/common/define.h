#ifndef DEFINE
#define DEFINE

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <functional>

#define WORKER_THREAD_COUNT 4
#define BUFFER_MAX_SIZE 65535

#define LOG_INFO(message) std::cout<<__FILE__<<":"<<__FUNCTION__<<":"<<__LINE__<<" [INF]"<<message<<std::endl;
#define LOG_ERR(message) std::cout<<__FILE__<<":"<<__FUNCTION__<<":"<<__LINE__<<" [ERR]"<<message<<std::endl;

typedef std::vector<char> ByteBuffer;
typedef std::shared_ptr<ByteBuffer> ByteBufferPtr;
typedef std::vector<ByteBufferPtr> BuffersVector;

enum class CodeCommand: uint8_t
{
    REGISTRATION = 1,
    LOGIN = 2,
    LOGOUT = 3,
    USER_LIST = 4,
    SEND_MESSAGE = 5,
    CONNECT_TO_USER = 6,
    DISCONNECT_FROM_USER = 7,
    ACCEPT_TO_CHAT = 8,
    SEE_REQUESTS = 9,
    ANSWER_ON_REQUEST = 10
};

std::ostream& operator <<(std::ostream& stream, const ByteBuffer& buffer);

struct User
{
    int id_;
    std::string name_;
    std::string nick_;
    bool operator==(const User &rhs) const {
            return rhs.name_ == name_;
        }
};


std::ostream& operator <<(std::ostream& stream, const User& user);

#endif // DEFINE

