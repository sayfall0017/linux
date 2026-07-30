#pragma once
#include<stdio.h>


#define MAX 1024
#define NONE_FLUSH (1 << 0)
#define LINE_FLUSH (1 << 1)
#define FULL_FLUSH (1 << 2)



typedef struct IO_FILE
{
    int fileno; //文件描述符
    int flag; //打开方式
    char outbuffer[MAX];//内容
    int bufferlen;//内容大小
    int flush_mothed;//刷新方式
}MYFILE;


MYFILE* MYfopen(const char *path, const char *mode);
void MYfclose(MYFILE* f);
int MYfwrite(MYFILE* f, void* str, int len);
void MYfflush(MYFILE* f);
