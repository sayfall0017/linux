

#include <iostream>
#include <cstring>
#include <memory>
#include "Inetaddr.hpp"
#include "Udpserver.hpp"
#include "Dict.hpp"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " server_port" << std::endl;
        return 1;
    }
    int port = std::stoi(argv[1]);

    Dict dict;
    dict.LoadDict();

    std::unique_ptr<UdpServer> usvr = std::make_unique<UdpServer>(port, [&dict](const std::string &word, InetAddr &client)
                                                                  { return dict.Translate(word, client); });
    usvr->Init();
    usvr->Strat();
    return 0;
}