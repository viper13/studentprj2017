#ifndef DEFINE
#define DEFINE

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <asio.hpp>
#include <functional>

#define WORKER_THREAD_COUNT 4

#define BUFFER_MAX_SIZE 65535

#define LOGIN_MESSAGE "$l"
#define GET_USER_LIST_MESSAGE "$g"
#define CREATE_CHAT_MESSAGE "$c"
#define SEND_MESSAGE "$s"
#define ERROR_MESSAGE "$e"
#define WAIT_FOR_NEXT_MESSAGE "$w"
#define DIRECT_MESSAGE "$d"
#define REQUEST_TO_CREATE_CHAT_MESSAGE "$r"
#define YES_MESSAGE "$y"
#define NO_MESSAGE "$n"
#define CHAT_MESSAGE "$C"
#define ADD_USER_TO_CHAT_MESSAGE "$A"

/*enum class CommandCode: uint8_t
{
    NONE,
    LOGIN_MESSAGE,
    GET_USER_LIST_MESSAGE,
    CREATE_CHAT_MESSAGE,
    SEND_MESSAGE,
    ERROR_MESSAGE,
    WAIT_FOR_NEXT_MESSAGE,
    DIRECT_MESSAGE,
    REQUEST_TO_CREATE_CHAT_MESSAGE,
    YES_MESSAGE,
    NO_MESSAGE,
    CHAT_MESSAGE,
    ADD_USER_TO_CHAT_MESSAGE

};*/


#define LOG_INFO(message) std::cout<<__FILE__<<":"<<__FUNCTION__<<":"<<__LINE__<<" [INF]"<<message<<std::endl;
#define LOG_ERR(message) std::cout<<__FILE__<<":"<<__FUNCTION__<<":"<<__LINE__<<" [ERR]"<<message<<std::endl;


typedef std::vector<char> ByteBuffer;
typedef std::shared_ptr<ByteBuffer> ByteBufferPtr;


std::ostream& operator<<(std::ostream& stream,const ByteBuffer& buffer);


struct User
{
    int id_;
    std::string name_;
    std::string nick_;
};

std::ostream& operator<<(std::ostream& stream,const User& buffer);

#endif // DEFINE
