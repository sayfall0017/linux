#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include "Task.hpp"
#include "Mutex.hpp"
#include "Log.hpp"
#include "Cond.hpp"
#include "Thread.hpp"

using namespace LogModule;
using namespace CondModule;
using namespace ThreadModule;
using namespace MutexModule;

namespace ThreadPoolModule
{

    static const int gnum = 5;
    template <typename T>
    class ThreadPool
    {
    private:
        void WakeUpAllThread()
        {
            LockGuard lockguard(_mutex);
            if (_sleepernum)
                _cond.Broadcast();
            LOG(LogLevel::INFO) << "唤醒所有的休眠线程";
        }

        void WakeUpOne()
        {
            _cond.Signal();
            LOG(LogLevel::INFO) << "唤醒一个休眠线程";
        }

        ThreadPool(int num = gnum, bool isrunning = false, int sleepernum = 0)
            : _num(num), _isrunning(isrunning), _sleepernum(sleepernum)
        {
            for (int i = 0; i < num; i++)
            {
                _threads.emplace_back(
                    [this]()
                    {
                        HandlerTask();
                    });
            }
        }

        bool Start()
        {
            if (_isrunning == true)
            {
                return true;
            }
            _isrunning = true;
            for (auto &thread : _threads) // 为什么用引用 》 不可拷贝
            {
                thread.Start();
                LOG(LogLevel::INFO) << "start new thread success:" << thread.Getname();
            }
            return true;
        }

        ThreadPool(const ThreadPool<T> &) = delete; // 单例模式 拷贝构造和赋值重载都不能用
        ThreadPool<T> &operator=(const ThreadPool<T> &) = delete;

    public:
        // 获取单例
        static ThreadPool<T> *Getinstance() // 静态成员函数 调用时候还不一定有ThreadPool对象
        {
            if (_inc == nullptr)
            {
                LockGuard lockguard(_lock);

                LOG(LogLevel::DEBUG) << "获取单例...";
                if (_inc == nullptr)
                {
                    LOG(LogLevel::DEBUG) << "首次适用单例，创建中...";
                    _inc = new ThreadPool<T>();
                    _inc->Start();
                }
            }
            return _inc;
        }

        void Stop()
        {
            if (!_isrunning)
                return;
            _isrunning = false;

            // 唤醒所有的线层
            WakeUpAllThread();
        }
        void Join()
        {
            for (auto &thread : _threads)
            {
                thread.Join();
            }
        }

        void HandlerTask()
        {
            char name[128];
            pthread_getname_np(pthread_self(), name, sizeof(name));
            while (true)
            {
                T t;
                {
                    LockGuard lockguard(_mutex);
                    // 1. a.队列为空 b. 线程池没有退出
                    while (_taskq.empty() && _isrunning)
                    {
                        _sleepernum++;
                        _cond.Wait(_mutex);
                        _sleepernum--;
                    }
                    // 2. 内部的线程被唤醒
                    if (!_isrunning && _taskq.empty())
                    {
                        LOG(LogLevel::INFO) << name << " 退出了, 线程池退出&&任务队列为空";
                        break;
                    }

                    // 一定有任务
                    t = _taskq.front(); // 从q中获取任务，任务已经是线程私有的了！！！
                    _taskq.pop();
                }
                t(); // 处理任务，不需要临界区内部处理
            }
        }

        bool Enqueue(const T &in)
        {
            if (_isrunning)
            {
                LockGuard lockguard(_mutex);
                _taskq.push(in);
                if (_threads.size() == _sleepernum)
                    WakeUpOne();
                return true;
            }
            return false;
        }

        ~ThreadPool()
        {
        }

    private:
        std::vector<Thread> _threads;
        int _num;
        std::queue<T> _taskq;
        Mutex _mutex;
        Cond _cond;

        bool _isrunning;
        int _sleepernum;

        static ThreadPool<T> *_inc;
        static Mutex _lock;
    };

    template <typename T>
    ThreadPool<T> *ThreadPool<T>::_inc = nullptr;

    template <typename T>
    Mutex ThreadPool<T>::_lock;
}