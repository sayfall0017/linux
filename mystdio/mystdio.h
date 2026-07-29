#pragma once
#include<stdio.h>


#define MAX 1024

typedef struct IO_FILE
{
    int fileno;
    int flag;
    char outbuffer[MAX];
    int bufferlen;
}MYFILE;


MYFILE* MYfopen(const char *path, const char *mode);
void MYfclose(MYFILE*);
int MYfwrite(MYFILE*, void* str, int len);
void MYfflush(MYFILE*);
