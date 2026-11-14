#pragma once
// 这个头文件的内容是实现UPD套接字的使用

#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <functional>
#include "Inetaddr.hpp"
#include "Log.hpp"

using namespace LogModule;

using func_t = std::function<std::string(const std::string &, InetAddr &)>;

class UdpServer
{
public:
    UdpServer(uint16_t port, func_t func)
        : _port(port), _func(func)
    {
    }
    void Init()
    {
        // 1.打开套接字（网络文件）
        size_t sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (sockfd < 0)
        {
            LOG(LogLevel::FATAL) << "socket fail";
            exit(1);
        }
        _sockfd = sockfd;
        LOG(LogLevel::INFO) << "socket success";

        // 2.bind
        // 填写结构体
        InetAddr local(_port);

        // bind
        int n = bind(_sockfd, CONV(local), local.NetAddrLen());
        if (n < 0)
        {
            LOG(LogLevel::FATAL) << "bind fail";
            exit(2);
        }
        LOG(LogLevel::INFO) << "bind success";
    }
    void Strat()
    {
        // 收发消息
        _running = true;
        while (_running)
        {
            // 收消息
            char buffer[1024];
            struct sockaddr_in peer;
            socklen_t len = sizeof(peer);
            int s = recvfrom(_sockfd, buffer, sizeof(buffer) - 1, 0, (struct sockaddr *)&peer, &len);
            if (s > 0)
            {
                uint16_t peer_port = ntohs(peer.sin_port);
                std::string peer_ip = inet_ntoa(peer.sin_addr);
                buffer[s] = 0;

                // 添加函数对象让功能和网络服务器发送分开
                InetAddr client(peer);
                std::string result = _func(buffer, client);

                // 发消息
                sendto(_sockfd, result.c_str(), result.size(), 0, CONV(client), client.NetAddrLen());
            }
        }
    }
    ~UdpServer()
    {
    }

private:
    uint16_t _port;
    size_t _sockfd;

    bool _running;
    func_t _func;
};