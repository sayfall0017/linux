#include <iostream>
#include <string>
#include <list>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "Mutex.hpp"
#include "Inetaddr.hpp"

using namespace MutexModule;

class Route
{
private:
    // 判断是不是新用户
    bool IsNewUser(InetAddr &peer)
    {
        for (auto u : _users)
        {
            if (peer == u)
                return false;
        }
        return true;
    }

    // 新增用户
    void AddUser(InetAddr &peer)
    {
        LOG(LogLevel::INFO) << "新增了一个用户：" << peer.StringAddr();
        _users.push_back(peer);
    }

    // 广播消息
    void BroadCast(const size_t &socket, const std::string &message, InetAddr &peer)
    {
        std::string send_message = peer.StringAddr() + " @ " + message;
        for (auto u : _users)
        {
            sendto(socket, send_message.c_str(), send_message.size(), 0, CONV(u), sizeof(u));
        }
    }

    // 判断并退出
    void Quit(const std::string &message, InetAddr &peer)
    {
        if (message == "QUIT")
        {
            for (auto iter = _users.begin(); iter != _users.end(); iter++)
            {
                if (peer == *iter)
                {
                    LOG(LogLevel::INFO) << "离线了一个用户：" << peer.StringAddr();
                    _users.erase(iter);
                    break;
                }
            }
        }
    }

public:
    Route()
    {
    }
    void MessageRoute(const size_t &socket, const std::string &message, InetAddr &peer)
    {
        // 访问的是共享资源（_users）
        LockGuard lockguard(_mutex);

        // 判断是不是新用户,是则加入
        if (IsNewUser(peer))
            AddUser(peer);

        // 将消息发送给所有人
        BroadCast(socket, message, peer);

        // 消息是QUIT就退出
        Quit(message, peer);
    }
    ~Route()
    {
    }

private:
    std::list<InetAddr> _users;
    Mutex _mutex;
};