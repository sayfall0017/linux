#include <iostream>
#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void ChildWrite(int wfd)
{
    char buffer[1024]; // 缓冲区
    int cnt = 0;
    while (true)
    {
        snprintf(buffer, sizeof(buffer), "I am Child, pid:%d, cnt:%d", getpid(), cnt++);
        write(wfd, buffer, strlen(buffer));
        sleep(1);
    }
}

void FatherWrite(int rfd)
{
    char buffer[1024];
    while(true)
    {
        buffer[0] = 0;
        ssize_t n = read(rfd, buffer, sizeof(buffer) - 1);
        if (n > 0)
        {
            buffer[n] = 0;
            std::cout << "Child say:" << buffer << std::endl;
        }
    }
}

int main()
{
    // 创建管道
    int fds[2] = {0};
    int n = pipe(fds);
    if (n < 0)
    {
        std::cerr << "pipe error" << std::endl;
        return 0;
    }
    std::cout << "fds[0]:" << fds[0] << std::endl; // 读端
    std::cout << "fds[1]:" << fds[1] << std::endl; // 写端

    // 创建子进程
    pid_t id = fork();
    if (id == 0) // 子进程
    {
        // code
        // 关闭不需要的读写端，形成通信通道
        // f -> r, c -> w
        close(fds[0]);
        ChildWrite(fds[1]);
        close(fds[1]);
        exit(1);
    }
    // father
    // 关闭不需要的读写端，形成通信通道
    // f -> r, c -> w
    close(fds[1]);
    FatherWrite(fds[0]);
    waitpid(id, nullptr, 0);
    close(fds[0]);
    return 0;
}