#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include<stdlib.h>


int main()
{
    printf("程序开始\n");
    pid_t id = fork();
    if(id == 0)
    {
        int cnt = 3;
        while(cnt)
        {
            printf("俺是熊二,pid:%d,ppid:%d\n",getpid(),getppid());
            sleep(1);
            cnt--;
        }
        exit(1);
    }
    else
    {
        int wstatus;
        pid_t child_pid = wait(&wstatus);
        while(1)
        {            
            printf("俺是熊妈妈，等待中...\n");
            if (WIFEXITED(wstatus)) 
            {
                printf("父进程：子进程(PID=%d)正常退出，退出码：%d\n", 
                    child_pid, WEXITSTATUS(wstatus));
                break;
            }
        }
    }
    return 0;
}
