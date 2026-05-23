#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>


int main()
{
  while(1)
  {
  sleep(1);
  printf("你好 咕咕咕 我是一个进程, 我的pid是：%d\n",getpid());
  }
  return 0;
}
