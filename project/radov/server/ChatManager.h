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
    void sendChatMessage(int idRoom, std::string message, std::string idClient);
    void requestMessage(std::string idClient, std::string idTarget, int room);
    int createChat(std::string idClient, std::string idTarget);
    void addUserToChatRoom(std::string idClient, int idRoom);

    std::string message_;
    std::vector<SessionManagerPtr> sessions_;

    void removeClient(std::string idClient);
    bool checkClientOnline(std::string idClient);

    std::vector<int> pullChatRooms(std::string idClient);

    void joinChat(int idRoom, std::string idClient);
    void getChatMessages(int idRoom, std::string idClient);
private:
    std::vector<ChatRoomPtr> chatRooms_;
    static ChatManager* p_instance;

    ChatManager();
    ChatManager(const ChatManager&);
    ChatManager& operator =(ChatManager&);

};

#endif // CHATMANAGER_H
