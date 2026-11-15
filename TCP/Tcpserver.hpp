#include <iostream>
#include <string>
#include <cstring>
#include "Log.hpp"
#include "Thread.hpp"
#include "Common.hpp"

int main(int argc, char *argv[])
{
    if(argc < 2)
    {
        std::cout << "Usage:" << argv[0] << "ServerIp ServerPort" << std::endl;
        exit(USAGE_ERR);
    }
    return 0;
}