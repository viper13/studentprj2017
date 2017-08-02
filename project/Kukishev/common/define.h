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
    DISCONNECT_FROM_USER,
    CONFIRM_TO_START_CHAT,
    SHOW_QUEUE_USERS,
    SING_UP,
    SHOW_CHATS,
    ENTER_CHAT,
    OUT_FROM_CHAT,
    MENU,
    ANSWER_ON_REQUEST_TO_CONNECT

};

#define WORKER_THREAD_COUNT 4
#define BUFFER_MAX_SIZE 65535

#define LOG_INFO(message) std::cout<<__FILE__<<":"<<__FUNCTION__<<":"<<__LINE__<<" [INF]"<<message<<std::endl;
#define LOG_ERR(message) std::cout<<__FILE__<<":"<<__FUNCTION__<<":"<<__LINE__<<" [ERR]"<<message<<std::endl;
#define USER_INFO(message) std::cout<<message<<std::endl;


typedef std::vector<uint8_t> ByteBuffer;
typedef std::shared_ptr<ByteBuffer> ByteBufferPtr;

std::ostream& operator<<(std::ostream& stream, const ByteBuffer& matrix);


struct NewUser
{
    int id;
    std::string name;
    std::string nick;
};

std::ostream& operator<<(std::ostream& stream, const NewUser& user);


#endif // DEFINE

