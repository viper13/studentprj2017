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

std::ostream& operator<<(std::ostream& stream, const ByteBuffer& buffer);

struct User
{
    int id_;
    std::string name_;
    std::string nick_;
};

std::ostream& operator<<(std::ostream& stream, const User& user);

#endif // DEFINE

