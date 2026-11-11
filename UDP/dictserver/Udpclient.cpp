#include <iostream>
#include <string>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main(int argc, char *argv[])
{
    // ./Udpclient ip port
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " server_ip server_port" << std::endl;
        return 1;
    }
    std::string server_ip = argv[1];
    uint16_t server_port = std::stoi(argv[2]);

    // 打开socket
    size_t sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        std::cout << "socket error" << std::endl;
        return 2;
    }
    std::cout << "socket success" << std::endl;

    //  填写 sockaddr_in 结构体
    struct sockaddr_in server;
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(server_ip.c_str());
    server.sin_port = htons(server_port);

    // bind
    // 说明一点：bind是bind自己的socket
    // 那么这里需要bind吗？
    // 必然是需要的，但是不需要显式bin
    
    // 首次发送消息，OS会自动给client进行bind，OS知道IP，端口号采用随机端口号的方式
    // （client每次都是率先发消息的，所以只需要确保自己的socket信息能够发出即可，并不在乎是哪个ip和port → 客户端的端口“是几”根本不重要）


    while(true)
    {
        std::string input;
        std::cout << "Please Enter# ";
        getline(std::cin, input);
        // 发消息
        int n = sendto(sockfd, input.c_str(), input.size(), 0, (sockaddr *)&server, sizeof(server));
        (void)n;
        // 收消息
        struct sockaddr_in peer;
        char buffer[1024];
        socklen_t len = sizeof(peer);
        int m = recvfrom(sockfd, buffer, sizeof(buffer), 0, (sockaddr *)&peer, &len);
        if (m > 0)
        {
            buffer[m] = 0;
            std::cout << buffer << std::endl;
        }
    }

    return 0;
}