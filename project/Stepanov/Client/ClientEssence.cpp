#include "ClientEssence.h"
#include "Client.h"


ClientEssence::ClientEssence(std::string address, std::string port)
    : Client(address,port)
    , hasRequest(false)
    , inChat(false)
    , isAuthorization(false)
    , isRegister(false)
    , isLogged(false)
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
                if(currentRoom!=0)
                {
                    write(Helper::makeAddMessage());
                }
                else
                {
                    LOG_INFO("You are not in room");
                }
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
            isLogged = true;
        }
        if(isAuthorization)
        {
            write(Helper::makeLoginMessage(message));
            isAuthorization=false;
            isLogged = true;
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
            else
            {
                LOG_INFO("You are not registered!");
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
            else
            {
                LOG_INFO("You are not registered!");
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
                      << "!setname  -- set name for your room\n"
                      << "!history  -- to see last 10 messages in chat\n"
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
        {//dva raza room ono idet po ppppppp
            if(!login.empty())
            {
                checkChangeRoom();
            }
            else
            {
                LOG_INFO("You are not loggined!");
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
        else if(currentRoom == 0)
        {
            LOG_INFO("Bad command! Type !help to see available commands");
        }
}

void ClientEssence::onRead(ByteBuffer data)
{
    std::string message(data.begin(), data.end());
    if(message.find(REQUEST_TO_CREATE_CHAT_MESSAGE)!=std::string::npos)
    {
        processRequestMessage(message);
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
        isAuthorization=true;
    }
    else if(message.find(CHAT_MESSAGE) != std::string::npos)
    {
        processChatMessage(message);
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

void ClientEssence::processChatMessage(std::string message)
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
        LOG_INFO("You have  "<<unReadMessages_.size()<<" unread messages");
    }
}

void ClientEssence::processRequestMessage(std::string message)
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

void ClientEssence::checkChangeRoom()
{
    int tempIdRoom = 0;
    std::string tempUserMessage;
    LOG_INFO("Enter id of room to enter");
    while (true) {
        std::cin >> tempUserMessage;
        tempIdRoom = atoi(tempUserMessage.c_str());
        if(tempIdRoom!=0)
        {
            break;
        }
        else
        {
            LOG_INFO("Wrong number! Try again!")
        }
    }
    currentRoom=tempIdRoom;
    inChat=true;
    write(Helper::makeRoomMessage(currentRoom));
}
