#ifndef DEFINE
#define DEFINE

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <set>
#include <map>
#include <memory>

#define BUFFER_MAX_SIZE 65535

#define WORKER_THREAD_COUNT 4

typedef  std::vector<char> ByteBuffer;
typedef std::shared_ptr<ByteBuffer> ByteBufferPtr;

typedef std::vector<std::string> StringBuffer;
typedef std::shared_ptr<StringBuffer> StringBufferPtr;

typedef std::set<std::string> StringSet;
typedef std::shared_ptr<StringSet> StringSetPtr;

std::ostream& operator<<(std::ostream& stream, const ByteBuffer& buffer);

#define LOG_INFO(message) std::cout<<__FILE__<<":"<<__FUNCTION__<<":"<<__LINE__<<" [INF]"<<message<<std::endl;
#define LOG_ERR(message) std::cout<<__FILE__<<":"<<__FUNCTION__<<":"<<__LINE__<<" [ERR]"<<message<<std::endl;

struct User
{
    int id_;
    std::string name_;
    std::string nick_;
};

std::ostream& operator<<(std::ostream& stream, const User& buffer);

#endif // DEFINE

