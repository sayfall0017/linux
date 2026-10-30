#pragma once

#include <iostream>
#include <string>
#include <filesystem>
#include <sstream>
#include <fstream>
#include "Mutex.hpp"

namespace LogModule
{
    using namespace MutexModule;

    const std::string gsep = "\r\n";
    class LogStrategy
    {
    public:
        virtual ~LogStrategy() = default;
        virtual void SyncLog(const std::string &massage) = 0;
    };

    class ConsoleLogStrategy : public LogStrategy
    {
    public:
        void SyncLog(const std::string &message) override
        {
            LockGuard lockGuard(_mutex);
            std::cout << message << gsep;
        }
        ~ConsoleLogStrategy()
        {
        }

    private:
        Mutex _mutex;
    };

    const std::string defaultpath = "./log";
    const std::string defaultfile = "my.log";

    class FileLogStrategy : public LogStrategy
    {
    public:
        FileLogStrategy(const std::string &path = defaultpath, const std::string &file = defaultfile)
            : _path(path),
              _file(file)
        {
            LockGuard lockGuard(_mutex);
            if (std::filesystem::exists(_path))
            {
                return;
            }
            try
            {
                // 监控区域
                std::filesystem::create_directories(_path);
            }
            catch (const std::filesystem::filesystem_error &e)
            {
                std::cerr << e.what() << "\n";
            }
        }

        void SyncLog(const std::string &message) override
        {
            LockGuard lockGuard(_mutex);

            std::string filename = _path + (_path.back() == '/' ? "" : "/") + _file;

            std::ofstream out(filename, std::ios::app);
            if (!out.is_open())
            {
                return;
            }
            out << message << gsep;
            out.close();
        }
        ~FileLogStrategy()
        {
        }

    private:
        std::string _path;
        std::string _file;
        Mutex _mutex;
    };
}