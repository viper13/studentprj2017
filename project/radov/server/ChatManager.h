#ifndef CHATMANAGER_H
#define CHATMANAGER_H

#include "Server.h"
#include "SessionWrapper.h"
#include "ChatRoom.h"

class ChatManager
{
public:
    static ChatManager &getInstance();
    void onConnected(SessionManagerPtr session);
    void getUserList(std::string idClient);
    void getMessageList(std::string idClient);
    void getChatsList(std::string idClient);
    void start(Server& server);
    void sendMessage(std::string idClient, std::string idTarget, std::string message);
    void sendChatMessage(int idRoom, std::string message, std::string idClient);
    void requestMessage(std::string idClient, std::string idTarget, int room);
    void createChat(std::string idClient, std::string idTarget);
    void addUserToChatRoom(std::string idClient, int idRoom);
    void disconnectUser(std::string idClient);

    std::string message_;
    std::vector<SessionManagerPtr> sessions_;

    int nextIdRoom;

    void removeClient(std::string idClient);
    bool checkClientOnline(std::string idClient);

private:
    std::vector<ChatRoomPtr> chatRooms_;
    static ChatManager* p_instance;

    ChatManager();
    ChatManager(const ChatManager&);
    ChatManager& operator =(ChatManager&);

};

#endif // CHATMANAGER_H
