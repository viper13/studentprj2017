#include "ClientAPI.h"


ClientAPI::ClientAPI(const std::__cxx11::string &address, const std::__cxx11::string &port)
{
    client_ = std::make_shared<Client>(address, port);
}
