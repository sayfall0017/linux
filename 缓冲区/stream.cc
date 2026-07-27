#include <cstdio>
#include <iostream>


int main()
{
    //向标准输出打印：stdout、cout
    std::cout << "hello cout" << std::endl;
    printf("hello stdout\n");

    //向标准错误打印：stderr、cerr
    std::cerr << "hello cerr" << std::endl;
    fprintf(stderr, "hello stderr\n");
    return 0;
}
