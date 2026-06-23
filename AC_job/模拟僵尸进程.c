#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>



int main()
{
    //模拟僵尸进程
    size_t id = fork();
    if(id == 0)
    {
        //子进程
        int cnt = 3;
        while(cnt--)
        {
            printf("我是一个子进程，我的PID：%d，我的PPID：%d\n",getpid(),getppid());
            sleep(1);
        }
        exit(1);
    }

    //父进程
    int cnt = 30;
    while(cnt--)
    {
        printf("我是一个父进程，我的PID：%d，我的PPID：%d\n",getpid(),getppid());
        sleep(1);
    }

    return 0;
}
