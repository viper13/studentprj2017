#include "ClientEssence.h"
#include "Client.h"


ClientEssence::ClientEssence(std::string address, std::string port)
    : Client(address,port)
    , hasRequest(false)
    , inChat(false)
    , isAuthorization(false)
    , isRegister(false)
    , currentRoom(0)
    , requestRoom(0)
{

}


void ClientEssence::processMessage(std::string message)
{
    std::stringstream UserStream(message);
    UserStream >> command;

        if(command.find("!register") != std::string::npos)
        {
            UserStream>>data;
            login = data;
            write(Helper::makeRegisterMessage(data));
        }
        else if(command.find("!list") != std::string::npos)
        {
            write(Helper::makeListMessage());
        }
        else if(command.find("!create") != std::string::npos)
        {    
            if(!login.empty())
            {
                UserStream >> data;
                write(Helper::makeCreateChatMessage(data));
                inChat = true;
            }
            else
            {
                LOG_INFO("You are not registered!");
            }
        }
        else if((command.find("!yes") != std::string::npos)&&(hasRequest))
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
        else if(command.find("!help") != std::string::npos)
        {
            showHelp();
        }
        else if(command.find("!history") != std::string::npos)
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
        else if(command.find("!roomlist") != std::string::npos)
        {
            write(Helper::makeRoomListMessage());
        }
        else if(command.find("!room") != std::string::npos)
        {
            if(!login.empty())
            {
                UserStream >> data;
                checkChangeRoom(data);
            }
            else
            {
                LOG_INFO("You are not loggined!");
            }
        }
        else if(command.find("!read") != std::string::npos)
        {
            for(std::string mess: unReadMessages_)
            {
                LOG_INFO(" Unread : "<<mess);
            }
            unReadMessages_.clear();
        }
        else if(command.find("!exit") != std::string::npos)
        {
            write(Helper::makeExitMessage());
            closeConnection();
        }
        else if(inChat)
        {
            if(command.find("!add") != std::string::npos)
            {
                if(currentRoom!=0)
                {
                    UserStream >> data;
                    write(Helper::makeAddMessage(data));
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
                    UserStream >> data;
                    write(Helper::makeSetRoomNameMessage(data));
                }
            }
            else
            {
                write(Helper::makeChatMessage(message));
            }
        }
        else if(isRegister)
        {
            write(Helper::makeCreateNewUserMessage(message));
            isRegister=false;
        }
        else if(isAuthorization)
        {
            write(Helper::makeLoginMessage(message));
            LOG_INFO(Helper::makeLoginMessage(message));
            isAuthorization=false;
        }
        else if(currentRoom==0)
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
        std::cout << message << std::endl;
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

void ClientEssence::checkChangeRoom(std::string message)
{
    int tempIdRoom = 0;
    tempIdRoom = atoi(message.c_str());
    if(tempIdRoom!=0)
    {
        currentRoom=tempIdRoom;
        inChat=true;
        write(Helper::makeRoomMessage(currentRoom));
    }
    else
    {
        LOG_INFO("Wrong number! Try again!")
    }
}

void ClientEssence::showHelp()
{

    std::cout << "------------------------------------------------------------\n"
              << " !list           -- to see who is online                   |\n"
              << " !roomlist       -- to get list of available rooms         |\n"
              << " !room [id]      -- change current room                    |\n"
              << " !create [user]  -- to create a chat                       |\n"
              << " !add [user]     -- to add preson into you current room    |\n"
              << " !read           -- to read unread messages                |\n"
              << " !setname [name] -- set name for your room                 |\n"
              << " !history        -- to see last 10 messages in chat        |\n"
              << " !exit           -- to close programm                      |\n"
              <<"-------------------------------------------------------------\n";
}
