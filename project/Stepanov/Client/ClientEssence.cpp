#include "ClientEssence.h"
#include "Client.h"


ClientEssence::ClientEssence(std::string address, std::string port)
    : Client(address,port)
    , hasRequest(false)
    , inChat(false)
    , isLogin(false)
    , isRegister(false)
    , currentRoom(0)
    , requestRoom(0)
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
                if(currentRoom == 0)
                {
                    LOG_INFO("You are not in room!");
                }
                else
                {
                    write(Helper::makeSetRoomNameMessage());
                }
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
            if(!login.empty())
            {
                write(Helper::makeCreateChatMessage());
                inChat = true;
            }
        }
        else if((message.find("!yes") != std::string::npos)&&(hasRequest))
        {
            if(!login.empty())
            {
                inChat=true;
                currentRoom = requestRoom;
                write(Helper::makeYesMessage(currentRoom));
            }
        }
        else if(message.find("!help") != std::string::npos)
        {
            LOG_INFO("You have next commands:");
            std::cout << "!list     -- to see who is online \n"
                      << "!roomlist -- to get list of available rooms\n"
                      << "!room     -- change current room\n"
                      << "!create   -- to create a chat\n"
                      << "!add      -- to add preson into you current room\n"
                      << "!read     -- to read unread messages\n"
                      << "!exit     -- to close programm\n";
        }
        else if(message.find("!history") != std::string::npos)
        {
            if(currentRoom == 0)
            {
                LOG_INFO("You are not in room!");
            }
            else
            {
                write(Helper::makeHistoryMessage());
            }
        }
        else if(message.find("!roomlist") != std::string::npos)
        {
            write(Helper::makeRoomListMessage());
        }
        else if(message.find("!room") != std::string::npos)
        {
            if(!login.empty())
            {
                LOG_INFO("Enter id of room to enter");
                std::cin >> currentRoom;
                inChat=true;
                write(Helper::makeRoomMessage(currentRoom));
            }
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

void ClientEssence::onRead()
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
            LOG_INFO("You have  "<<unReadMessages_.capacity()<<" unread messages");
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
