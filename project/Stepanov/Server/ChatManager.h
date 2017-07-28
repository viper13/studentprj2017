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

    bool authFunction(std::string login);

    void registerNewUser(std::string login, std::string nick);
    bool loginIntoUser(std::string login , std::string pass);

    void getUserList(char idClient);
    void getUserList(std::string loginClient);

    void start(Server& server);

    void debug();

    void sendMessage(char idClient, char idTarget, std::string message);

    void sendChatMessage(int idRoom, std::string message, char idClient);
    void sendChatMessage(int idRoom, std::string message, std::string loginClient);

    void requestMessage(char idClient, char idTarget, std::string message, int room);
    void requestMessage(std::string loginClient, std::string loginTarget, std::string message, int room);

    int createChat();

    void addUserToChatRoom(char idClient, int idRoom);
    void addUserToChatRoom(std::string loginClient, int idRoom);

    void sendMessagesHistory(int idRoom, std::string userLogin);

    void enterChat(int idRoom,std::string userLogin);

    void removeUser(std::string userLogin);

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
