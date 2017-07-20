#include <iostream>
#include <asio.hpp>
#include "messageManager.h"
#include "worker.h"

int main(int argc, char *argv[])
{
    MessageManager *manager = new MessageManager();
    manager->initializeSession();
    manager->comunicateWithUser();
    manager->closeSession();
    delete manager;
    return 0;
}

