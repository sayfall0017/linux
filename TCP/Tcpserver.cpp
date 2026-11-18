#include "Common.hpp"
#include "Tcpserver.hpp"
#include "Dict.hpp"
#include "Inetaddr.hpp"

int main(int argc, char *argv[])
{
    if(argc < 2)
    {
        std::cout << "Usage:" << argv[0] << "ServerIp ServerPort" << std::endl;
        exit(USAGE_ERR);
    }
    std::string Port = argv[1];

    Dict dict;
    dict.LoadDict();

    std::unique_ptr<TcpServer> tsvr = std::make_unique<TcpServer>(Port, [&dict](const std::string &word, InetAddr &client)
                                                       {
                                                           return dict.Translate(word, client);
                                                       });
    tsvr->Init();
    tsvr->Run();
    return 0;
}