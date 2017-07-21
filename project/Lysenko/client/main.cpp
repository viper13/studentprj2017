#include "Worker.h"
#include "CommandClient.h"
#include "define.h"

int main(int argc, char *argv[])
{
    /*std::string str("Name");
    str = BufferConverter::addOpCode(Operation::logIn, str);
    std::cout << "Message [" << str << "]\nOperation code: " << static_cast<int>( static_cast<uint8_t>(str[0]) )<< "\nData [" << str.substr( 1, str.size() ) << "]";*/

    std::shared_ptr<CommandClient> clientPtr( new CommandClient("127.0.0.1", "1122") );
    clientPtr->start();

    Worker::instance()->startThreads();

    std::string userName;
    std::cin >> userName;

    clientPtr->logIn(userName);

    Worker::instance()->joinThreads();

//    std::shared_ptr<MessageClient> clientPtr(new MessageClient("127.0.0.1", "1122"));
//    clientPtr->start();

//    Worker::instance()->startThreads();
//    LOG_INFO("Client started!");

//    std::string message;

//    bool needStop = false;
//    while (!needStop)
//    {
//        LOG_INFO("Enter message: ");

//        std::getline(std::cin, message);
//        clientPtr->write(message);

//        needStop = ( message == "stop" );
//    }

//    Worker::instance()->joinThreads();

//    LOG_INFO("Client finished!");

    return 0;
}

