#pragma once


#include <iostream>
#include <vector>
#include <time.h>

//////////////////////////////////////////////////
typedef void (*task_t)();

void Printlog()
{
    std::cout << "这是一个打印日志的任务" << std::endl;
}

void Download()
{
    std::cout << "这是一个下载的任务" << std::endl;
}

void Upload()
{
    std::cout << "这是一个上传的任务" << std::endl;
} 

//////////////////////////////////////////////////

class TaskManager
{
public:
    TaskManager()
    {
        srand(time(nullptr));
    }
    void Register(task_t t)
    {
        _tasks.emplace_back(t);
    }
    size_t Code()
    {
        if (_tasks.empty()) return -1;
        return rand() % _tasks.size();
    }
    void Execute(int code)
    {
        if(code >= 0 && code < _tasks.size())
        {
            _tasks[code]();
        }
    }
    ~TaskManager()
    {}
private:
    std::vector<task_t> _tasks;
};