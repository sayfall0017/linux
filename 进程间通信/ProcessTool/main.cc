#include "ProcessPool.hpp"


int main()
{
    // 1. 创建进程池
    ProcessPool pp(gdefaultnum);
    // 2. 启动进程池
    pp.Start();

    // 3. 派发任务运行
    int cnt = 10;
    while(cnt--)
    {
        pp.Run();
        sleep(1);
    } 

    // 4. 回收进程池
    pp.Stop();

    return 0;
}