#include <string.h>
#include "mystdio.h"

int main()
{
    MYFILE* file = MYfopen("log.txt","w");
    if(file->fileno < 0)
    {
        printf("fopen error");
        return 1;
    }

    char* msg = "hello MYfwrite";
    MYfwrite(file, msg, strlen(msg));


    MYfclose(file);
    return 0;
}
