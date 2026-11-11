#pragma once

#include <pthread.h>
#include <iostream>
#include <string>
#include <functional>

namespace ThreadModule
{
    static uint32_t number = 1;
    class Thread
    {
        using func_t = std::function<void()>;

    private:
        void EnableRunning()
        {
            _isRunning = true;
        }
        void EnableDetach()
        {
            _isDetached = true;
        }
        static void *Routine(void *args) // 需要执行的操作是利用传入的this指针使用传入的函数
        {
            Thread *self = static_cast<Thread *>(args);
            self->EnableRunning();
            if (self->_isDetached)
                self->Detach();
            pthread_setname_np(self->_tid, self->_name.c_str());
            self->_func();

            return nullptr; //  不传递任何信息，本意指的是join捕获的类似退出码
        }

    public:
        Thread(func_t func) // 传入函数
            : _tid(0),
              _isRunning(false),
              _isDetached(false),
              res(nullptr),
              _func(func)
        {
            _name = "Thread-" + std::to_string(number++);
        }

        std::string Getname()
        {
            return _name;
        }

        bool Start()
        {
            if (_isRunning)
                return false;
            int n = pthread_create(&_tid, nullptr, Routine, (void *)this);
            if (n == 0)
                return true;
            else
                return false;
        }

        void Detach()
        {
            if (_isDetached)
                return;
            if (_isRunning)
                pthread_detach(_tid);
            EnableDetach();
        }

        bool Stop()
        {
            if (_isRunning)
            {
                int n = pthread_cancel(_tid);
                if (n == 0)
                {
                    _isRunning = false;
                    return true;
                }
                else
                    return false;
            }
            else
                return false;
        }

        void Join()
        {
            if(_isDetached)
                return;
            int n = pthread_join(_tid, &res);
            if(n == 0)
                std::cout << "join回收成功" << std::endl;//
            else
                std::cout << "join失败" << std::endl;
        }

        ~Thread()
        {
        }

    private:
        std::string _name;
        pthread_t _tid;
        bool _isRunning;
        bool _isDetached;
        void *res;
        func_t _func;
    };
}
