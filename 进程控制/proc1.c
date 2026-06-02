#include<stdio.h>
#include<stdlib.h>


int main()
{
  FILE *fp = fopen("test.txt","r");
  if(fp == NULL) return 13;   //有错误，一定返回13
  printf("normal");
  fclose(fp);


  return 0;
}

