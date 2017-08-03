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
    CONNECT_TO_USER = 4,
    ACCEPT_TO_CHAT = 5,
    START_CHAT = 6,
    SEND_MESSAGE = 7,
    DISCONNECT_FROM_USER = 8,
    SEE_REQUESTS = 9,
    SEE_FRIENDS = 10,
    USER_LIST = 11,
    PRINT_HELP = 12,
    EXIT = 13,
    NONE = 14
};

std::ostream& operator <<(std::ostream& stream, const ByteBuffer& buffer);

struct User
{
public:
    bool operator==(const User &rhs) const {
            return rhs.name_ == name_;
        }

    //getters
    int getID() const;
    std::string getName() const;
    std::string getNick() const;

    //setters
    void setID(int newId);
    void setName(std::string newName);
    void setNick(std::string newNick);
private:
    int id_;
    std::string name_;
    std::string nick_;
};


std::ostream& operator <<(std::ostream& stream, const User& user);

#endif // DEFINE

