#include <iostream>
#include <cstring>
#include <memory>
#include <functional>
#include "Udpserver.hpp"
#include "Route.hpp"
#include "ThreadPool.hpp"

using task_t = std::function<void()>;

using namespace ThreadPoolModule;
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " server_port" << std::endl;
        return 1;
    }
    int port = std::stoi(argv[1]);

    Route route;

    auto tp = ThreadPool<task_t>::GetInstance();

    std::unique_ptr<UdpServer>
        usvr = std::make_unique<UdpServer>(port, [&tp, &route](const size_t &socket, const std::string &message, InetAddr &client)
            {
                task_t t= std::bind(&Route::MessageRoute,&route,socket, message, client);
                tp -> Enqueue(t);
             });
    usvr->Init();
    usvr->Strat();
    return 0;
}