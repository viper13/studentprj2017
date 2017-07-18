#include "Worker.h"
#include "define.h"
#include "MessageManager.h"

int main(int argc, char *argv[])
{
    MessageManager mng("127.0.0.1", "1122");

    mng.start();
    //LOG_INFO("Threads FINISHED!!!");
    return 0;
}

