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


#define LOG_INFO(message) std::cout<<"\n [INFO]"<<message<<std::endl;
#define LOG_ERR(message) std::cout<<__FILE__<<":"<<__FUNCTION__<<":"<<__LINE__<<"\n [ERR]"<<message<<std::endl;


typedef std::vector<char> ByteBuffer;
typedef std::shared_ptr<ByteBuffer> ByteBufferPtr;

using namespace boost;



std::ostream& operator<<(std::ostream& stream,const ByteBuffer& buffer);


#endif // DEFINE
