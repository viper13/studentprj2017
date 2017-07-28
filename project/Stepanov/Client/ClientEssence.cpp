#include "ClientEssence.h"
#include "Client.h"


ClientEssence::ClientEssence(std::string address, std::string port)
    : Client(address,port)
    , hasRequest(false)
    , inChat(false)
    , isLogin(false)
    , isRegister(false)
{

}


void ClientEssence::processMessage(std::string message)
{

        if(inChat)
        {
            if(message.find("!add") != std::string::npos)
            {
                LOG_INFO("Enter id of target:");
                std::cin >>message;
                message=ADD_USER_TO_CHAT_MESSAGE+message;
                LOG_INFO(message<<"eeeeee");
                write(message);
            }
            else
            {
                message=CHAT_MESSAGE+message;
                write(message);
            }
        }
        if(isRegister)
        {
            message=CREATE_NEW_USER+message;
            write(message);
            isRegister=false;
        }
        if(isLogin)
        {
            message=LOGIN_INTO_ACCOUNT+message;
            write(message);
            isLogin=false;
        }

        if(message.find("!register") != std::string::npos)
        {
            LOG_INFO("Enter your id(1 character)");
            std::cin >> message;
            login = message;
            idClient=message[0];
            message = LOGIN_MESSAGE+login;
            write(message);
            LOG_INFO(" Type !help to see commands");
        }
        else if(message.find("!list") != std::string::npos)
        {
            message = GET_USER_LIST_MESSAGE;
            write(message);
        }
        else if(message.find("!create") != std::string::npos)
        {
            LOG_INFO("Enter id of target to create chat");
            std::cin >> message;
            message=CREATE_CHAT_MESSAGE+message;
            write(message);
            inChat = true;
        }
        else if(message.find("!server") != std::string::npos)
        {
            LOG_INFO("Enter message");
            std::cin >> message;
            write(message);
        }
        else if(message.find("!direct") != std::string::npos)
        {
            char idTarget;
            std::string send;
            LOG_INFO("Enter id of target");
            std::cin >> idTarget;
            LOG_INFO("Enter a message to send");
            std::cin >> send;
            message=DIRECT_MESSAGE;
            message+=idTarget;
            message+=send;
            LOG_INFO("message from client "<<message);
            write(message);
        }
        else if((message.find("!yes") != std::string::npos)&&(hasRequest))
        {
            LOG_INFO("You accepted chat request!");
            inChat=true;
            message=YES_MESSAGE;
            message+=std::to_string(currentRoom);
            write(message);
        }
        else if(message.find("!help") != std::string::npos)
        {
            LOG_INFO("You have next commands:");
            std::cout << "!list -- to see who is online \n"
                      << "!server -- to write direct message to server\n"
                      << "!direct -- to write direct message to user\n"
                      << "!enter  -- to enter chat room\n"
                      << "!roomlist -- to get list of available rooms\n"
                      << "!room   -- change current room\n"
                      << "!create -- to create a chat\n";
        }
        else if(message.find("!history") != std::string::npos)
        {
            message=GET_CHAT_HISTORY;
            write(message);
        }
        else if(message.find("!enter") != std::string::npos)
        {
            message=ENTER_CHAT_MESSAGE;
            LOG_INFO("Enter id room to enter!");
            std::cin >> currentRoom;
            message+=std::to_string(currentRoom);
            inChat=true;
            write(message);
        }
        else if(message.find("!roomlist") != std::string::npos)
        {
            message = GET_ROOM_LIST_MESSAGE;
            write(message);
        }
        else if(message.find("!room") != std::string::npos)
        {
            LOG_INFO("Enter id of room to enter");
            std::cin >> currentRoom;
            message = SET_ROOM_MESSAGE+std::to_string(currentRoom);
            write(message);
        }
        else if(message.find("!read") != std::string::npos)
        {
            for(std::string mess: unReadMessages_)
            {
                LOG_INFO(" Unread : "<<mess);
            }
            unReadMessages_.clear();
        }
        else if(message.find("!exit") != std::string::npos)
        {
            message=EXIT_MESSAGE;
            write(message);
            closeConnection();
        }

}

void ClientEssence::onRead(ByteBuffer data)
{
    std::string message(buffer_.begin(), buffer_.end());
    if(message.find(REQUEST_TO_CREATE_CHAT_MESSAGE)!=std::string::npos)
    {
        message.erase(message.begin(),message.begin()+2);
        currentRoom=atoi(message.c_str());
        LOG_INFO("Type !yes to create chat!\n");
        hasRequest = true;
    }
    else if(message.find(CREATE_NEW_USER) != std::string::npos)
    {
        message.erase(message.begin(),message.begin()+2);
        LOG_INFO(message);
        isRegister=true;
    }
    else if(message.find(LOGIN_INTO_ACCOUNT) != std::string::npos)
    {
        message.erase(message.begin(),message.begin()+2);
        LOG_INFO(message);
        isLogin=true;
    }
    else if(message.find(CHAT_MESSAGE) != std::string::npos)
    {
        message.erase(message.begin(),message.begin()+2);
        int idRoom;
        idRoom = message[0] - '0';//remake
        if(idRoom == currentRoom)
        {
            LOG_INFO("message from chat "<<idRoom);
            LOG_INFO(message);
        }
        else
        {
            unReadMessages_.push_back(message);
            LOG_INFO("unreadMessage capacity "<<unReadMessages_.capacity());
        }

    }
    else if(message.find(CREATE_CHAT_MESSAGE) != std::string::npos)
    {
        message.erase(message.begin(),message.begin()+2);
        currentRoom = atoi(message.c_str());
    }
    else if(message.find(GET_ROOM_LIST_MESSAGE) != std::string::npos)
    {
        message.erase(message.begin(),message.begin()+2);
        LOG_INFO("Available rooms id: \n"<<message);
    }
    else
    {
        LOG_INFO(message);
    }
}
