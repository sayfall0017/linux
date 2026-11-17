
#include "Common.hpp"


using namespace LogModule;
using namespace ThreadModule;
using namespace ThreadPoolModule;

const int defaultsockfd = -1;

using func_t = std::function<std::string(const std::string &, InetAddr &)>;

class TcpServer : public NoCopy
{
public:
    TcpServer(const uint16_t port, func_t func) : _port(port), _listensocket(defaultsockfd), _running(false), _func(func)
    {
    }
    void Init()
    {
    }
    void Start()
    {
    }
    ~TcpServer()
    {
    }

private:
    uint16_t _port;
    int _listensocket;
    bool _running;

    func_t _func;
};
