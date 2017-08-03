#include <iostream>
#include <asio.hpp>
#include "MessageManager.h"

int main(int argc, char *argv[])
{
    std::shared_ptr<MessageManager> manager(new MessageManager());
    manager->initializeSession();
    manager->inputMessage();
    manager->closeSession();

    return 0;
}

