#ifndef CHATSESSION_H
#define CHATSESSION_H

#include "Session.h"
#include "User.h"

class ChatSession : public Session
{
public:
    ChatSession();

    static std::shared_ptr<ChatSession> getNewSessions();

    virtual void onRead(ByteBuffer buffer) override;
    virtual void onDisconected() override;

    void setReadHandle(const std::function<void (std::shared_ptr<ChatSession>, ByteBufferPtr)> &value);
    void setDisconectedHandle(const std::function<void (std::shared_ptr<ChatSession>)> &disconectedHandle);
    void execute(CommandCode code, ByteBufferPtr data);
    User &getUser();
    void sendMessageToClient(const std::string& text);

private:

    std::shared_ptr<ChatSession> getPtrFromThis();

    std::function<void (std::shared_ptr<ChatSession>, ByteBufferPtr)> readHandle_;
    std::function<void (std::shared_ptr<ChatSession>)> disconectedHandle_;

    User user_;
};

typedef std::shared_ptr<ChatSession> ChatSessionPtr;

#endif // SESSIONCHAT_H
