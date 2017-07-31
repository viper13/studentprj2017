#ifndef USER_H
#define USER_H

#include "string"

struct User
{
    bool isLogin_ = false;
    std::string name_ = "";
    //
    bool isInChat_ = false;
};

#endif // USER_H
