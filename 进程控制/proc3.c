#include<stdio.h>
#include<stdlib.h>

void func()
{
  printf("熊大：熊二你别想出来了！\n");
  exit(1);
  printf("exit()已经把熊二关起来了！\n");
}

int main()
{
  func();

  printf("俺是熊二，救救俺！\n");
  return 0;
}

