#include "SessionEssence.h"
#include "ChatManager.h"
SessionEssence::SessionEssence()
    :Session()
    , hasRequest(false)
{

}
ChatManager& c = ChatManager::getInstance();
std::shared_ptr<SessionEssence> SessionEssence::getNewSession()
{
    SessionEssencePtr session = std::make_shared<SessionEssence>();
    return session;
}

void SessionEssence::onRead(ByteBuffer data)
{
    std::string message(buffer_.begin(), buffer_.end());

    if(message.find(LOGIN_MESSAGE) != std::string::npos)
    {
        message.erase(message.begin(),message.begin()+2);
        if(!(c.checkUserOnline(message)))
        {
            login = message;
            if(c.authFunction(message))
            {
                message=LOGIN_INTO_ACCOUNT;
                message+="That name already registered!Please enter a password!";
                write(message);
            }
            else
            {
                message=CREATE_NEW_USER;
                message+="You a new user! Please enter a your password!";
                write(message);
            }
        }
        else
        {
            write("That user already online");
        }
    }
    else if(message.find(LOGIN_INTO_ACCOUNT) != std::string::npos)
    {
        message.erase(message.begin(),message.begin()+2);
        if(c.loginIntoUser(login,message))
        {
            message="Welcome back " + login;
            write(message);
            availableRooms = c.regainChatRooms(login);
            if(!availableRooms.empty())
            {
                write(" You joined you previus rooms. Type !roomslist to see more");
            }
        }
        else
        {
            message=LOGIN_INTO_ACCOUNT;
            message+="Wrong password! Try again!";
            write(message);
        }

    }
    else if(message.find(CREATE_NEW_USER) != std::string::npos)
    {
        message.erase(message.begin(),message.begin()+2);
        c.registerNewUser(login ,message);
        std::string send = "Welcome to chat ";
        send+=login;
        send+="! Type !help to see commands!";
        write(send);
    }
    else if(message.find(GET_USER_LIST_MESSAGE) != std::string::npos)
    {
        c.getUserList(login);
    }
    else if(message.find(CREATE_CHAT_MESSAGE) != std::string::npos)
    {
        message.erase(message.begin(),message.begin()+2);
        targetLogin=message;
        currentRoom = c.createChat();
        availableRooms.push_back(currentRoom);
        hasRequest=true;
        c.requestMessage(login,targetLogin,currentRoom);
        c.addUserToChatRoom(login,currentRoom);
        message=CREATE_CHAT_MESSAGE;
        message+=std::to_string(currentRoom);
        write(message);
    }
    else if((message.find(YES_MESSAGE) != std::string::npos)&&(hasRequest))
    {
        message.erase(message.begin(),message.begin()+2);
        std::stringstream ss(message);
        ss >> currentRoom;
        ss.get();
        availableRooms.push_back(currentRoom);
        c.addUserToChatRoom(login,currentRoom);
    }
    else if(message.find(CHAT_MESSAGE) != std::string::npos)
    {
        c.sendChatMessage(currentRoom,message,login);
    }
    else if(message.find(ADD_USER_TO_CHAT_MESSAGE) != std::string::npos)
    {
        message.erase(message.begin(),message.begin()+2);
        targetLogin=message;
        c.requestMessage(login,targetLogin,currentRoom);
    }
    else if(message.find(GET_CHAT_HISTORY) != std::string::npos)
    {
        c.sendMessagesHistory(currentRoom,login);
    }
    else if(message.find(GET_ROOM_LIST_MESSAGE) != std::string::npos)
    {
        std::string answer;
        answer = GET_ROOM_LIST_MESSAGE;
        for(int i:availableRooms)
        {
            answer+=std::to_string(i);
            answer+=" <-id    ";
            answer+=c.getRoomName(i);
            answer+=" <-name";
            answer+="\n";
        }
        write(answer);
    }
    else if(message.find(SET_ROOM_MESSAGE) != std::string::npos)
    {
        message.erase(message.begin(),message.begin()+2);
        currentRoom = atoi(message.c_str());
        write("You room changed to "+message);
    }
    else if(message.find(EXIT_MESSAGE) != std::string::npos)
    {
        c.removeUser(login);
        LOG_INFO("Removed user "<<login);
    }
    else if(message.find(SET_ROOM_NAME) != std::string::npos)
    {
        message.erase(message.begin(),message.begin()+2);
        c.setRoomName(message,currentRoom,login);
    }

}

void SessionEssence::onUnexpectedClose()
{
    if(login.empty())
        login = "Unregistered user";
    c.removeUser(login);
}

void SessionEssence::setInChat(bool value)
{
    inChat = value;
}

void SessionEssence::setHasRequest(bool value)
{
    hasRequest = value;
}

std::string SessionEssence::getTargetLogin() const
{
    return targetLogin;
}

std::string SessionEssence::getLogin() const
{
    return login;
}
