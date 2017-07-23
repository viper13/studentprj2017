#ifndef CHATMANAGER_H
#define CHATMANAGER_H

#include "Server.h"
#include "SessionEssence.h"
#include "ChatRoom.h"

class ChatManager
{
public:

    static ChatManager &getInstance();

    void onConnected(SessionEssencePtr session);

    void getUserList(char idClient);

    void start(Server& server);

    void debug();

    void sendMessage(char idClient, char idTarget, std::string message);

    void sendChatMessage(int idRoom, std::string message, char idClient);

    void requestMessage(char idClient, char idTarget, std::string message, int room);

    void createChat();

    void addUserToChatRoom(char idClient, int idRoom);

    std::string message_;

    std::vector<SessionEssencePtr> sessions_;

    int nextIdRoom;

private:

    std::vector<ChatRoomPtr> chatRooms_;
    static ChatManager * p_instance;
    ChatManager();
    ChatManager(const ChatManager&);
    ChatManager& operator =(ChatManager&);

};

#endif // CHATMANAGER_H
