#include <iostream>
#include <cstring>
#include <memory>
#include "Udpserver.hpp"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " server_port" << std::endl;
        return 1;
    }
    int port = std::stoi(argv[1]);

    std::unique_ptr<UdpServer>
        usvr = std::make_unique<UdpServer>(port, [](){});
    usvr->Init();
    usvr->Strat();
    return 0;
}