#ifndef COMMANDCLIENT_H
#define COMMANDCLIENT_H

#include "MessageClient.h"

class CommandClient
        : public MessageClient
{
    public:

        CommandClient(std::string address, std::string port);

        void tryLogIn(std::string userName);
        void disconnect(std::string userName);

        void setOnReadCallBack(std::function< void(std::string) > onReadCallback) noexcept;

        virtual ~CommandClient() = default;

    private:

        virtual void onRead(const ByteBuffer& buffer) override;

        std::function< void(std::string) > onReadCallback_;
};

#endif // COMMANDCLIENT_H
