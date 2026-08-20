#pragma once

#include <iostream>
#include <cstdlib>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "task.hpp"

class Channel
{
public:
    Channel(int fd, pid_t id)
        : _wfd(fd),
          _subid(id)
    {
        _name = "channel-" + std::to_string(_wfd) + "-" + std::to_string(_subid);
    }
    void Send(int taskcode)
    {
        // 发送的实质是写入管道
        int n = write(_wfd, &taskcode, sizeof(taskcode));
        (void)n;
    }
    void Close()
    {
        close(_wfd);
    }
    void Wait()
    {
        pid_t rid = waitpid(_subid, nullptr, 0);
        (void)rid;
    }
    int Wfd()
    {
        return _wfd;
    }
    pid_t subid()
    {
        return _subid;
    }
    std::string name()
    {
        return _name;
    }

    ~Channel() {}

private:
    int _wfd;
    pid_t _subid;
    std::string _name;
};

class ChannelManager
{
public:
    ChannelManager()
        : _next(0)
    {
    }
    void Insert(int fd, pid_t id)
    {
        _channels.emplace_back(fd, id);
    }
    Channel &Select()
    {
        auto &c = _channels[_next++];
        _next %= _channels.size();
        return c;
    }
    void StopSubProcess()
    {
        for (auto &c : _channels)
        {
            c.Close();
            std::cout << "关闭了子进程：" << c.name() << std::endl;
        }
    }
    void WaitSubProcess()
    {
        for (auto &c : _channels)
        {
            c.Wait();
            std::cout << "回收了子进程：" << c.name() << std::endl;
        }
    }
    ~ChannelManager() {}

private:
    std::vector<Channel> _channels;
    int _next;
};

const int gdefaultnum = 5;

class ProcessPool
{
public:
    ProcessPool(int num) : _process_num(num)
    {
        _tm.Register(Printlog);
        _tm.Register(Download);
        _tm.Register(Upload);
    }
    void work(int fd)
    {
        while (true)
        {
            int code = 0;
            int n = read(fd, &code, sizeof(code));
            if (n > 0)
            {
                if (n != sizeof(code))
                {
                    continue;
                }
                std::cout << "子进程[pid:" << getpid() << "]收到任务码：" << code << std::endl;
                // 执行任务
                _tm.Execute(code);
            }
            else if (n == 0)
            {
                std::cout << "EOF，子进程退出" << std::endl;
                break;
            }
            else
            {
                std::cout << "读取错误，进程退出" << std::endl;
                break;
            }
        }
    }
    bool Start()
    {
        std::cout.flush(); 
        for (int i = 0; i < _process_num; i++)
        {
            // 1. 创建管道
            int fds[2] = {0};
            int n = pipe(fds);
            if (n < 0)
                return false;

            // 2. 创建子进程
            pid_t subid = fork(); // 父进程返回subid,子进程返回0
            if (subid > 0)        // father
            {
                // 关闭读端
                close(fds[0]);
                // 父进程不需要关闭写端，close(fds[1]);
                _cm.Insert(fds[1], subid);
            }
            else if (subid == 0) // child
            {
                // 关闭写端
                close(fds[1]);
                work(fds[0]); // 没有得到任务就会阻塞
                close(fds[0]);
                exit(0);
            }
            else
                return false;
        }
        return true;
    }

    void Run() // 运行，主动分发程序
    {


        // 1. 选择一个任务
        int taskcode = _tm.Code();
        // 选择通道
        auto &c = _cm.Select();
        std::cout << "选择了一个子进程：" << c.name() << std::endl;
        // 发送命令
        c.Send(taskcode);
        std::cout << "发送了命令，命令码为：" << taskcode << std::endl;
    }

    void Stop()
    {
        // 关闭写通道就会自动关闭读
        _cm.StopSubProcess();
        // 回收了子进程
        _cm.WaitSubProcess();
    }
    ~ProcessPool() {}

private:
    ChannelManager _cm;
    int _process_num;
    TaskManager _tm;
};