#include <stdio.h>
#include <iostream>
#include <string.h>
#include <unistd.h>


int main()
{
    printf("hello printf\n");
    fprintf(stdout, "hello fprintf\n");
    const char* s = "hello fwrite\n";
    fwrite(s, strlen(s), 1, stdout);

    const char* ss = "hello write\n";
    write(1, ss, strlen(ss));
    
    //？？？
    fork();
//    //向标准输出打印：stdout、cout
//    std::cout << "hello cout" << std::endl;
//    printf("hello stdout\n");
//
//    //向标准错误打印：stderr、cerr
//    std::cerr << "hello cerr" << std::endl;
//    fprintf(stderr, "hello stderr\n");
//    return 0;
}
