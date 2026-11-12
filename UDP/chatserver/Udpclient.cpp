#include <iostream>
#include <string>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "Thread.hpp"
#include "Inetaddr.hpp"

using namespace ThreadModule;

int sockfd = 0;
std::string server_ip;
uint16_t server_port = 0;
pthread_t id;

void Send()
{
    InetAddr server(server_ip, server_port);
    while (true)
    {
        std::string input;
        std::cout << "Please Enter# ";
        getline(std::cin, input);
        // 发消息
        int n = sendto(sockfd, input.c_str(), input.size(), 0, CONV(server), server.NetAddrLen());
        (void)n;

        if (input == "QUIT")
        {
            pthread_cancel(id);
            break;
        }
    }
}
void Recv()
{
    // 收消息
    while (true)
    {
        struct sockaddr_in peer;
        char buffer[1024];
        socklen_t len = sizeof(peer);
        int m = recvfrom(sockfd, buffer, sizeof(buffer), 0, (sockaddr *)&peer, &len);
        if (m > 0)
        {
            buffer[m] = 0;
            std::cerr << buffer << std::endl;
        }
    }
}

int main(int argc, char *argv[])
{
    // ./Udpclient ip port
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " server_ip server_port" << std::endl;
        return 1;
    }
server_ip = argv[1];
server_port = std::stoi(argv[2]);

    // 打开socket
sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        std::cout << "socket error" << std::endl;
        return 2;
    }
    std::cout << "socket success" << std::endl;

    // 客户端改用多线程解决阻塞时只能处理一个用户的bug
    Thread sender(Send);
    Thread recver(Recv);

    sender.Start();
    recver.Start();

    id = recver.Id();

    sender.Join();
    recver.Join();

    return 0;
}