#include "SessionEssence.h"
#include "ChatManager.h"
SessionEssence::SessionEssence()
    :Session()
{

}
ChatManager& c = ChatManager::getInstance();
std::shared_ptr<SessionEssence> SessionEssence::getNewSession()
{
    SessionEssencePtr session = std::make_shared<SessionEssence>();
    return session;
}

char SessionEssence::getIdClient()
{
    return idClient;
}

void SessionEssence::onRead(ByteBuffer data)
{
    std::string message(buffer_.begin(), buffer_.end());

    if(message.find(LOGIN_MESSAGE) != std::string::npos)
    {
        idClient = message[2];
        LOG_INFO("Registered client with id = "<<idClient);
        write("You succesesfully registered!");

    }
    else if(message.find(GET_USER_LIST_MESSAGE) != std::string::npos)
    {
        c.getUserList(idClient);

    }
    else if(message.find(CREATE_CHAT_MESSAGE) != std::string::npos)
    {
        idTarget=message[2];
        LOG_INFO("User "<<idClient<<" wish to create chat with " << idTarget<<" !");
    }
    else if(message.find(DIRECT_MESSAGE) != std::string::npos)
    {
        idTarget=message[2];
        std::string send(message.begin()+3,message.end());
        LOG_INFO("message on server side"<<send);
        c.sendMessage(idClient,idTarget,send);
    }
}

