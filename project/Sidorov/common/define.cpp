#include "define.h"

std::ostream& operator <<(std::ostream& stream, const ByteBuffer& buffer)
{
    std::string str (buffer.begin(), buffer.end());
    stream << "[" << str << "]";
    return stream;
}

std::ostream& operator <<(std::ostream& stream, const User& user)
{
    stream << "User{ id: " << user.getID()
           << " name: [" << user.getName()
           << "] nick: [" << user.getNick() << "]}";
    return stream;
}

int User::getID() const
{
    return id_;
}

std::string User::getName() const
{
    return name_;
}

std::string User::getNick() const
{
    return nick_;
}

void User::setID(int newId)
{
    id_ = newId;
}

void User::setName(std::string newName)
{
    name_ = newName;
}

void User::setNick(std::string newNick)
{
    nick_ = newNick;
}
