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
    NONE = 0,
    LOGIN_MESSAGE = 1,
    GET_USER_LIST_MESSAGE = 2,
    CREATE_CHAT_MESSAGE = 3,
    SEND_MESSAGE = 4,
    ERROR_MESSAGE = 5,
    WAIT_FOR_NEXT_MESSAGE = 6,
    DIRECT_MESSAGE = 7,
    REQUEST_TO_CREATE_CHAT_MESSAGE = 8,
    YES_MESSAGE = 9,
    NO_MESSAGE = 10,
    CHAT_MESSAGE = 11,
    ADD_USER_TO_CHAT_MESSAGE = 12

};*/


#define LOG_INFO(message) std::cout<<__FILE__<<":"<<__FUNCTION__<<":"<<__LINE__<<" [INF]"<<message<<std::endl;
#define LOG_ERR(message) std::cout<<__FILE__<<":"<<__FUNCTION__<<":"<<__LINE__<<" [ERR]"<<message<<std::endl;


typedef std::vector<char> ByteBuffer;
typedef std::shared_ptr<ByteBuffer> ByteBufferPtr;


std::ostream& operator<<(std::ostream& stream,const ByteBuffer& buffer);


#endif // DEFINE
