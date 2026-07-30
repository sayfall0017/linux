#include "mystdio.h"
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>



MYFILE* BuyFile(int fd, int flag)
{
    // 这个函数是用来开空间填充节点的
    // 开空间
    MYFILE* f = (MYFILE*)malloc(sizeof(MYFILE));
    if(f == NULL) return 0;

    // 填节点
    f->fileno = fd;
    f->flag = flag;
    f->bufferlen = 0;
    f->flush_mothed = LINE_FLUSH; //默认显示器 行刷新
    memset(f->outbuffer, 0, sizeof(f->outbuffer));
    return f;
}

MYFILE* MYfopen(const char *path, const char *mode)
{
    int flag = 0;
    int fd = -1;
    //用open打开
    if(strcmp(mode, "w") == 0)
    {
        flag = O_CREAT | O_WRONLY | O_TRUNC;
        fd = open(path, flag, 0666);
    }
    else if(strcmp(mode, "a") == 0)
    {
        flag = O_CREAT | O_WRONLY | O_APPEND;
        fd = open(path, flag, 0666);
    }
    else if(strcmp(mode, "r") == 0)
    {
        flag = O_RDONLY;
        fd = open(path, flag);
    }
    else
    {//...
    }


    if(fd < 0) return NULL;
    // 已经打开了一个文件，接下来把文件用双向链表管理起来
    return BuyFile(fd, flag);
}

void MYfclose(MYFILE* f)
{
    if(f->fileno < 0) return ;
    MYfflush(f);
    close(f->fileno);
    free(f);
}


int MYfwrite(MYFILE* f, void* str, int len)
{
    //1. 刷新 即拷贝
    memcpy(f->outbuffer + f->bufferlen, str, len);
    f->bufferlen += len;
    //2. 判断是否满足刷新条件
    if((f->flush_mothed & LINE_FLUSH) && f->outbuffer[f->bufferlen-1] == '\n')
    {
        MYfflush(f);
    }
    return 0;
}


void MYfflush(MYFILE* f)
{
    int n = write(f->fileno, f->outbuffer, f->bufferlen);
    if (n == -1) 
    {
       // 处理错误（如记录日志、清理资源）
       perror("write failed");
       return; // 或根据需求处理
    }
    f->bufferlen = 0;
}
