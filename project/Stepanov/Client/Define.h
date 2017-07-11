#ifndef DEFINE_H
#define DEFINE_H


#include <iostream>
#include <vector>
#include <string>

#define WORKER_THREAD_COUNT 4

#define BUFFER_MAX_SIZE 65535

#define LOG_INFO(message) std::cout <<__FILE__<<";"<<__FUNCTION__<<";"<<__LINE__<<"\n [INFO] "<<message<<std::endl;

#define LOG_ERR(message) std::cout <<__FILE__<<";"<<__FUNCTION__<<";"<<__LINE__<<"\n [ERR] "<<message<<std::endl;

typedef std::vector<char> ByteBuffer;

#endif // DEFINE_H
