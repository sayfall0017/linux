
#include "Common.hpp"
#include "Inetaddr.hpp"

using namespace LogModule;
using namespace ThreadModule;
using namespace ThreadPoolModule;

const static int defaultsockfd = -1;
const static int backlog = 8;

using func_t = std::function<std::string(const std::string &, InetAddr &)>;

class TcpServer : public NoCopy
{
public:
    TcpServer(const uint16_t port, func_t func) : _port(port), _listensockfd(defaultsockfd), _running(false), _func(func)
    {
    }
    void Init()
    {
        // socket 
        _listensockfd = socket(AF_INET, SOCK_STREAM, 0);
        if(_listensockfd < 0)
        {
            LOG(LogLevel::FATAL) << "SOCKET ERROR";
            exit(SOCKET_ERR);
        }
        LOG(LogLevel::INFO) << "socket success: " << _listensockfd; // 
        // 打开了socket
        // bind 不显式绑定
        InetAddr local(_port);
        int n = bind(_listensockfd, CONV(local), local.NetAddrLen());
        if(n < 0)
        {
            LOG(LogLevel::FATAL) << "BIND ERROR";
            exit(BIND_ERR);
        }
        LOG(LogLevel::INFO) << "bind success: " << _listensockfd; //
        // listen
        n = listen(_listensockfd, backlog);
        if (n < 0)
        {
            LOG(LogLevel::FATAL) << "LISTEN ERROR";
            exit(LISTEN_ERR);
        }
        LOG(LogLevel::INFO) << "listen success: " << _listensockfd; // 此时就可以连接了
    }
    void Run()
    {
        _running = true;
        while(true)
        {
            sockaddr_in peer;
            socklen_t len = sizeof(peer);
            // accept
            int sockfd = accept(_listensockfd, CONV(peer), &len);
            if (sockfd < 0)
            {
                LOG(LogLevel::WARNING) << "ACCEPT ERROR";
                continue;
            }
            InetAddr addr(peer);
            LOG(LogLevel::INFO) << "accept success, peer addr : " << addr.StringAddr();
            
        }
    }
    ~TcpServer()
    {
    }

private:
    uint16_t _port;
    int _listensockfd;
    bool _running;

    func_t _func;
};
