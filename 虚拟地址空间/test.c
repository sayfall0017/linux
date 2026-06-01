#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>

int g_val = 0;

int main()
{
  int id = fork();
  if(id == 0)
  {
    while(1)
    {
      printf("子进程---g_val:%d---&g_val:%p---pid:%d---ppid:%d\n",g_val,&g_val,getpid(),getppid());
      g_val++;
      sleep(1);
    }
  }
  else
  {
    while(1)
    {  
      printf("父进程---g_val:%d---&g_val:%p---pid:%d---ppid:%d\n",g_val,&g_val,getpid(),getppid());
      sleep(1);
    }
  }
  return 0;
}
