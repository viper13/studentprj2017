#include <iostream>
#include <asio.hpp>
#include "Worker.h"
#include "Client.h"
#include "define.h"
#include "MessageManager.h"

int main(int argc, char *argv[])
{
    MessageManager* manager = new MessageManager();
    manager->initializeSession();
    manager->inputMessage();
    manager->closeSession();

    return 0;
}

