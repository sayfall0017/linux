#pragma once
// 这个头文件的内容是实现UPD套接字的使用

#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <functional>
#include "Log.hpp"

using namespace LogModule;

using func_t = std::function<std::string(const std::string&, sockaddr_in&)>;

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
        if(sockfd == 0)
        {
            LOG(LogLevel::FATAL) << "socket fail";
            exit(1);
        }
        _sockfd = sockfd;
        LOG(LogLevel::DEBUG) << "socket success";


        // 2.bind
        // 填写结构体
        struct sockaddr_in local;
        bzero(&local, sizeof(local));
        local.sin_port = htons(_port);
        // local.sin_addr.s_addr = inet_addr(_ip.c_str());//TODO
        local.sin_addr.s_addr = INADDR_ANY;
        local.sin_family = AF_INET;

        // bind
        int n = bind(_sockfd, (struct sockaddr *)&local, sizeof(local));
        // 绑定问题->ip问题
        if(n < 0)
        {
            LOG(LogLevel::FATAL) << "bind fail";
            exit(2);
        }
        LOG(LogLevel::DEBUG) << "bind success";
    }
    void Strat()
    {
        // 收发消息
        _running = true;
        while(_running)
        {
            // 收消息
            char buffer[1024];
            struct sockaddr_in peer;
            socklen_t len = sizeof(peer);
            int s = recvfrom(_sockfd, buffer, sizeof(buffer) - 1, 0, (struct sockaddr *)&peer, &len);
            if(s == 0)
            {
                LOG(LogLevel::FATAL) << "recvfrom fail";
                exit(3);
            }



            uint16_t peer_port = ntohs(peer.sin_port);
            std::string peer_ip = inet_ntoa(peer.sin_addr);
            buffer[s] = 0;


            // LOG(LogLevel::DEBUG) << "[" << peer_ip << ":" << peer_port << "]" << buffer;
            // 发消息
            // std::string echo_string = "server echo@";
            // echo_string += buffer;

            // 添加函数对象让功能和网络服务器发送分开
            std::string result = _func(buffer, peer);

            sendto(_sockfd, result.c_str(), result.size(), 0, (struct sockaddr *)&peer, len);
        }
    }
    ~UdpServer()
    {
    }

private:
    uint16_t _port;
    // std::string _ip; // 点分十进制
    size_t _sockfd;
    
    bool _running;
    func_t _func;
};