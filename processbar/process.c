#include<stdio.h>
#include<string.h>
#include<unistd.h>

#define NUM 101
#define STALE '#'


void process_v1()
{
  char buffer [NUM];
  memset(buffer, 0, sizeof(buffer));
  const char* label = "/-|\\";
  int len = strlen(label);


  int cnt = 0;
  while(cnt <= 100)
  {
    printf("[%-100s][%3d%%][%c]\r",buffer,cnt,label[cnt%len]);
    fflush(stdout);
    buffer[cnt] = STALE;
    cnt++;
    usleep(50000);
  }
  printf("\n");
}


int main()
{
  process_v1();
  return 0;
}
