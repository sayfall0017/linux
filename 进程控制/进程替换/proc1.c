#include<unistd.h>
#include<stdio.h>

int main()
{
  printf("父进程开始了\n");
  execl("/usr/bin/ls","ls","-a","-l",NULL);
  printf("我被替换了\n");

  return 0;
}

