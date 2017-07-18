#ifndef COMMANDCLIENT_H
#define COMMANDCLIENT_H

#include "MessageClient.h"

class CommandClient
        : public MessageClient
{
    public:

        CommandClient(std::string address, std::string port);
        //CommandClient(std::string address, std::string port, std::string userName);

        std::string getUserName();
        void setUserName(std::string& newUserName);
        bool isLoggedIn();
        void setLogInStatus(bool newLogInStatus);
        bool isInChatRoom();
        void setChatRoomStatus(bool newChatRoomStatus);

        void start();
        void logIn(std::string userName);

        virtual ~CommandClient() = default;

    private:

        void writeOperation(Operations operationCode);
        void handleWriteOperation( BuffersVector data,
                                   asio::error_code error,
                                   size_t writtenBytesCount );

        std::string userName_;
        bool isLoggedIn_;
        bool isInChatRoom_;
};

#endif // COMMANDCLIENT_H
