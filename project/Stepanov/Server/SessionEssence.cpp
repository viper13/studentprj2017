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
    std::string message(data.begin(), data.end());
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
        processLoginInteAccMessage(message);
    }
    else if(message.find(CREATE_NEW_USER) != std::string::npos)
    {
        message.erase(message.begin(),message.begin()+2);
        processCreateNewUserMessage(message);
    }
    else if(message.find(GET_USER_LIST_MESSAGE) != std::string::npos)
    {
        c.getUserList(login);
       // c.debug();
    }
    else if(message.find(CREATE_CHAT_MESSAGE) != std::string::npos)
    {
        processCreateChatMessage(message);
    }
    else if((message.find(YES_MESSAGE) != std::string::npos)&&(hasRequest))
    {
        message.erase(message.begin(),message.begin()+2);
        processYesMessage(message);
    }
    else if(message.find(CHAT_MESSAGE) != std::string::npos)
    {
        c.sendChatMessage(currentRoom,message,login);
    }
    else if(message.find(ADD_USER_TO_CHAT_MESSAGE) != std::string::npos)
    {
        message.erase(message.begin(),message.begin()+2);
        processAddUserToChatMessage(message);
    }
    else if(message.find(GET_CHAT_HISTORY) != std::string::npos)
    {
        c.sendMessagesHistory(currentRoom,login);
    }
    else if(message.find(GET_ROOM_LIST_MESSAGE) != std::string::npos)
    {
        processGetRoomListMessage();
    }
    else if(message.find(SET_ROOM_MESSAGE) != std::string::npos)
    {
        processSetRoomMessage(message);
    }
    else if(message.find(EXIT_MESSAGE) != std::string::npos)
    {
        c.removeUser(login);
        LOG_INFO("Removed user "<<login);
    }
    else if(message.find(SET_ROOM_NAME) != std::string::npos)
    {
        message.erase(message.begin(),message.begin()+2);
        processSetRoomNameMessage(message);
    }
}

void SessionEssence::onUnexpectedClose()
{
    if(login.empty())
        login = "Unregistered user";
    c.removeUser(login);
}

void SessionEssence::processSetRoomMessage(std::string message)
{
    message.erase(message.begin(),message.begin()+2);
    int tempIdRoom;
    tempIdRoom = atoi(message.c_str());
    bool find;
    for(int i:availableRooms)
    {
        if(tempIdRoom==i)
        {
            currentRoom=tempIdRoom;
            find=true;
            write("You room changed to "+message);
        }
    }
    if(!find)
    {
        write("That room is not available for you");
    }
}

void SessionEssence::processCreateNewUserMessage(std::string message)
{
    c.registerNewUser(login ,message);
    c.addUserToChatRoom(login,1);
    availableRooms.push_back(1);
    std::string send = "Welcome to chat ";
    send+=login;
    send+="! Type !help to see commands!";
    write(send);
}

void SessionEssence::processLoginInteAccMessage(std::string message)
{
    if(c.loginIntoUser(login,message))
    {
        message="Welcome back " + login;
        write(message);
        availableRooms = c.regainChatRooms(login);
        if(!availableRooms.empty())
        {
            write("You joined you previus rooms. Type !roomslist to see more");
        }
    }
    else
    {
        message=LOGIN_INTO_ACCOUNT;
        message+="Wrong password! Try again!";
        write(message);
    }
}

void SessionEssence::processCreateChatMessage(std::string message)
{
    message.erase(message.begin(),message.begin()+2);
    targetLogin=message;
    if(targetLogin.compare(login)==0)
    {
        write("You can not create chat room with yourself!");
        return;
    }
    if(c.checkUserOnline(targetLogin))
    {
            hasRequest=true;
            currentRoom = c.createChat();
            availableRooms.push_back(currentRoom);
            c.requestMessage(login,targetLogin,currentRoom);
            c.addUserToChatRoom(login,currentRoom);
            message=CREATE_CHAT_MESSAGE;
            message+=std::to_string(currentRoom);
            write(message);

    }
    else
    {
        write("That user is not online! You can't create chat room!");
    }

}

void SessionEssence::processAddUserToChatMessage(std::string message)
{
    targetLogin=message;
    if(c.checkUserOnline(targetLogin))
    {
        if(!(c.checkUserInChat(targetLogin,currentRoom)))
        {
            c.requestMessage(login,targetLogin,currentRoom);
        }
        else
        {
            write("That user already in chat");
        }
    }
}

void SessionEssence::processYesMessage(std::string message)
{
    std::stringstream ss(message);
    ss >> currentRoom;
    ss.get();
    availableRooms.push_back(currentRoom);
    c.addUserToChatRoom(login,currentRoom);
}

void SessionEssence::processGetRoomListMessage()
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

void SessionEssence::processSetRoomNameMessage(std::string message)
{
    if(currentRoom!=1)
    {
        c.setRoomName(message,currentRoom,login);
    }
    else
    {
        write("You can not change General Chat name!");
    }
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
