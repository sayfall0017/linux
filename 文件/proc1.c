#include<stdio.h>


int main()
{
    FILE* pf = fopen("test.txt","w");
    if(pf != NULL)
    {
        fputs("linux is so easy", pf);
        
        char buffer[128];
        fgets(buffer, sizeof(buffer), pf);
        fputs(buffer,stdout);
        fclose(pf);
    }
    return 0;
}
