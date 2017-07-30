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
                write(Helper::makeAddMessage());
            }
            else if(message.find("!setname") != std::string::npos)
            {
                write(Helper::makeSetRoomNameMessage());
            }
            else
            {
                write(Helper::makeChatMessage(message));
            }
        }
        if(isRegister)
        {
            write(Helper::makeCreateNewUserMessage(message));
            isRegister=false;
        }
        if(isLogin)
        {
            write(Helper::makeLoginMessage(message));
            isLogin=false;
        }

        if(message.find("!register") != std::string::npos)
        {
            LOG_INFO("Enter your user name");
            std::cin >> message;
            login = message;
            write(Helper::makeRegisterMessage(message));
        }
        else if(message.find("!list") != std::string::npos)
        {
            write(Helper::makeListMessage());
        }
        else if(message.find("!create") != std::string::npos)
        {
            write(Helper::makeCreateChatMessage());
            inChat = true;
        }
        else if(message.find("!direct") != std::string::npos)
        {
            //not maked. Is it good idea?
        }
        else if((message.find("!yes") != std::string::npos)&&(hasRequest))
        {
            inChat=true;
            currentRoom = requestRoom;
            write(Helper::makeYesMessage(currentRoom));
        }
        else if(message.find("!help") != std::string::npos)
        {
            LOG_INFO("You have next commands:");
            std::cout << "!list     -- to see who is online \n"
                      << "!roomlist -- to get list of available rooms\n"
                      << "!room     -- change current room\n"
                      << "!create   -- to create a chat\n"
                      << "!exit     -- to close programm\n";
        }
        else if(message.find("!history") != std::string::npos)
        {
            write(Helper::makeHistoryMessage());
        }
        else if(message.find("!roomlist") != std::string::npos)
        {
            write(Helper::makeRoomListMessage());
        }
        else if(message.find("!room") != std::string::npos)
        {
            LOG_INFO("Enter id of room to enter");
            std::cin >> currentRoom;
            inChat=true;
            write(Helper::makeRoomMessage(currentRoom));
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
            write(Helper::makeExitMessage());
            closeConnection();
        }
}

void ClientEssence::onRead(ByteBuffer data)
{
    std::string message(buffer_.begin(), buffer_.end());
    if(message.find(REQUEST_TO_CREATE_CHAT_MESSAGE)!=std::string::npos)
    {
        message.erase(message.begin(),message.begin()+2);
        std::stringstream ss(message);
        ss >> requestRoom;
        ss.get();
        message.erase(message.begin(),message.begin()+1);
        LOG_INFO(message);
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
        std::stringstream ss(message);
        ss >> idRoom;
        ss.get();
        if(idRoom == currentRoom)
        {
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
