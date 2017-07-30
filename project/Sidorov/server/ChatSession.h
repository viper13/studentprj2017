#ifndef CHATSESSION_H
#define CHATSESSION_H

#include "Session.h"

class ChatRoom;

class ChatSession : public Session
{
public:
    ChatSession();

    static std::shared_ptr<ChatSession> getNewSession();
    void onRead(ByteBuffer data) override;
    void setHandleRead(std::function<void (std::shared_ptr<ChatSession>, ByteBufferPtr)> handle);
    //getters
    std::string getUserName() const;
    bool getisLogged() const;
    bool getisInChat() const;
    std::string getisChatWith() const;
    //setters
    void setUserName(std::string newName);
    void setisLogged(bool newState);
    void setisInChat(bool newState);
    void setisChatWith(std::string newState);

    int findChatPos(std::shared_ptr<ChatRoom> chatRoom);
    void execute(CodeCommand code, ByteBufferPtr data);
    void sendMessageToClient(const std::string &text);
    std::vector<std::shared_ptr<ChatRoom> > chatRoomsSession;
    void disconnectedFromAll();
private:
    std::function<void (std::shared_ptr<ChatSession>, ByteBufferPtr)> handleRead_;
    std::string userName_;
    std::string isChatWith_;
    bool isLogged_;
    bool isInChat_;

    std::shared_ptr<ChatSession> getPointer();

};

typedef std::shared_ptr<ChatSession> ChatSessionPtr;

#endif // CHATSESSION_H
