#include <iostream>
#include <cstring>
#include <memory>
#include "Udpserver.hpp"

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " server_ip server_port" << std::endl;
        return 1;
    }

    std::unique_ptr<UdpServer> usvr = std::make_unique<UdpServer>(argv[1],std::stoi(argv[2]));
    usvr->Init();
    usvr->Strat();
    return 0;
}