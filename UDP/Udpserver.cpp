#include <iostream>
#include <memory>
#include "Udpserver.hpp"

int main()
{

    std::unique_ptr<UdpServer> usvr = std::make_unique<UdpServer>();
    usvr->Init();
    usvr->Strat();
    return 0;
}