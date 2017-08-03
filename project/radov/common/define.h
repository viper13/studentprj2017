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


enum class Commands: uint8_t
{
    LOGIN_MESSAGE,
    GET_USER_LIST_MESSAGE,
    CREATE_CHAT_MESSAGE,
    SEND_MESSAGE,
    ERROR_MESSAGE,
    WAIT_FOR_NEXT_MESSAGE,
    REQUEST_TO_CREATE_CHAT_MESSAGE,
    YES_MESSAGE,
    NO_MESSAGE,
    CHAT_MESSAGE,
    ADD_USER_TO_CHAT_MESSAGE,
    AUTHORIZATION_FAILED,
    AUTHORIZATION_SUCCESS,
    GET_MESSAGE_LIST,
    GET_CHATS_LIST,
    SET_ROOM,
    EXIT
};


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

struct ChatMessage
{
    int id_;
    std::string chat_id_;
    std::string user_id_;
    std::string message_;
};

std::ostream& operator<<(std::ostream& stream,const User& buffer);

#endif // DEFINE
