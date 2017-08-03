#include "CommandManager.h"

int main(int argc, char *argv[])
{
    CommandManager cm;

    cm.createClient();
    cm.startClient();

    return 0;
}

