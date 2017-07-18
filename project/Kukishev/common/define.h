#ifndef DEFINE
#define DEFINE

#include <iostream>
#include <string>
#include <vector>
#include <memory>

enum class CommandCode: uint8_t
{
    LOGIN,
    LOGOUT,
    USER_LIST,
    SEND_MESSAGE,
    CONNECT_TO_USER,
    DISCONNECT_TO_USER,
    ANSWER_TO_USER
};

#define WORKER_THREAD_COUNT 4
#define BUFFER_MAX_SIZE 65535

#define LOG_INFO(message) std::cout<<__FILE__<<":"<<__FUNCTION__<<":"<<__LINE__<<" [INF]"<<message<<std::endl;
#define LOG_ERR(message) std::cout<<__FILE__<<":"<<__FUNCTION__<<":"<<__LINE__<<" [ERR]"<<message<<std::endl;



typedef std::vector<uint8_t> ByteBuffer;
typedef std::shared_ptr<ByteBuffer> ByteBufferPtr;

std::ostream& operator<<(std::ostream& stream, const ByteBuffer& matrix);

#endif // DEFINE

