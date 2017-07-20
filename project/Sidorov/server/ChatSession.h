#ifndef CHATSESSION_H
#define CHATSESSION_H

#include "Session.h"

class ChatSession : public Session
{
    std::function<void (std::shared_ptr<ChatSession>, ByteBufferPtr)> handleRead_;
    std::string userName_;
    bool isLogged;
    bool isinChat;
    std::shared_ptr<ChatSession> getPointer();
public:
    ChatSession();

    static std::shared_ptr<ChatSession> getNewSession();
    void onRead(ByteBuffer data) override;
    void setHandleRead(std::function<void (std::shared_ptr<ChatSession>, ByteBufferPtr)> handle);
    //getters
    std::string getUserName() const;
    bool getisLogged() const;
    bool getisInChat() const;
    //setters
    void setUserName(std::string newName);
    void setisLogged(bool newState);
    void setisInChat(bool newState);
};

typedef std::shared_ptr<ChatSession> ChatSessionPtr;

#endif // CHATSESSION_H
