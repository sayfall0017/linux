#include<stdio.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<string.h>
int main()
{
    int fp = open("sayfall.txt", O_CREAT | O_RDWR | O_TRUNC, 0666);
    if (fp == -1)
    {
        perror("open failed");
        return 1; // 错误退出
    }



    char buffer[128];
    strcpy(buffer, "I like linux!");
    int len = strlen(buffer);
    write(fp, buffer, len);
    char buffer2[128];
    lseek(fp, 0, SEEK_SET);
    read(fp, buffer2, len);
    write(2, buffer2, len);
    close(fp);
    return 0;
}
