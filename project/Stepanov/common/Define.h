#ifndef DEFINE
#define DEFINE

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <boost/asio.hpp>
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
#define CREATE_NEW_USER "$2"
#define LOGIN_INTO_ACCOUNT "$1"
#define GET_CHAT_HISTORY "$H"
#define ENTER_CHAT_MESSAGE "$E"
#define GET_ROOM_LIST_MESSAGE "$L"
#define SET_ROOM_MESSAGE "$S"
#define EXIT_MESSAGE "$X"
#define SET_ROOM_NAME "$R"


#define LOG_INFO(message) std::cout<<"\n[INFO] "<<message<<std::endl;
#define LOG_ERR(message) std::cout<<__FILE__<<":"<<__FUNCTION__<<":"<<__LINE__<<"\n[ERR] "<<message<<std::endl;


typedef std::vector<char> ByteBuffer;
typedef std::shared_ptr<ByteBuffer> ByteBufferPtr;

using namespace boost;

std::ostream& operator<<(std::ostream& stream,const ByteBuffer& buffer);


#endif // DEFINE
