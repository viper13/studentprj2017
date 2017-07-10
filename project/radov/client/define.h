#ifndef DEFINE
#define DEFINE

#include <iostream>
#include <string>
#include <vector>

#define WORKER_THREAD_COUNT 4

#define LOG_INFO(message) std::cout<<__FILE__<<":"<<__FUNCTION__<<":"<<__LINE__<<" [INF]"<<message<<std::endl;
#define LOG_ERR(message) std::cout<<__FILE__<<":"<<__FUNCTION__<<":"<<__LINE__<<" [ERR]"<<message<<std::endl;

typedef std::vector<char> ByteBuffer;

#endif // DEFINE

