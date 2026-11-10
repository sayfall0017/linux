#pragma once
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
// 网络地址和主机地址之间进行转换的类

class InetAddr
{
public:
    InetAddr(struct sockaddr_in &addr) : _addr(addr)
    {
        _port = ntohs(_addr.sin_port);   // 从网络中拿到的！网络序列
        _ip = inet_ntoa(_addr.sin_addr); // 4字节网络风格的IP -> 点分十进制的字符串风格的IP
    }
    uint16_t Port() { return _port; }
    std::string Ip() { return _ip; }

    ~InetAddr()
    {
    }

private:
    struct sockaddr_in _addr;
    std::string _ip;
    uint16_t _port;
};