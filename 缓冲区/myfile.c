#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>


int main()
{
    close(1);
    int fd = open("myfile.txt", O_CREAT | O_WRONLY | O_APPEND, 0666);
    // 库函数
    printf("fd: %d\n",fd);
    printf("hello world\n");
    printf("hello world\n");
    printf("hello world\n");
    
    fflush(stdout);
    // 系统调用
    const char *msg = "hello write!\n";
    write(fd, msg, strlen(msg));
    
    close(fd);
    return 0;
}
