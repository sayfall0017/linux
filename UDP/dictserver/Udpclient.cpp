#include <iostream>
#include <string>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "Inetaddr.hpp"

int main(int argc, char *argv[])
{
    // ./Udpclient ip port
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " server_ip server_port" << std::endl;
        return 1;
    }
    std::string server_ip = argv[1];
    uint16_t server_port = std::stoi(argv[2]);

    // 打开socket
    size_t sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        std::cout << "socket error" << std::endl;
        return 2;
    }
    std::cout << "socket success" << std::endl;

    //  填写 sockaddr_in 结构体
    InetAddr server(server_ip, server_port);



    while(true)
    {
        std::string input;
        std::cout << "Please Enter# ";
        getline(std::cin, input);
        // 发消息
        int n = sendto(sockfd, input.c_str(), input.size(), 0, CONV(server), server.NetAddrLen());
        (void)n;
        // 收消息
        struct sockaddr_in peer;
        char buffer[1024];
        socklen_t len = sizeof(peer);
        int m = recvfrom(sockfd, buffer, sizeof(buffer), 0, (sockaddr *)&peer, &len);
        if (m > 0)
        {
            buffer[m] = 0;
            std::cout << buffer << std::endl;
        }
    }

    return 0;
}