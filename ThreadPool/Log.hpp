#pragma once

#include <iostream>
#include <string>
#include <filesystem>
#include <sstream>
#include <fstream>
#include <memory>
#include <unistd.h>
#include <ctime>
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

    ///////////////////////////////////////////////////////////
    enum class LogLevel
    {
        DEBUG,
        INFO,
        WARNING,
        ERROR,
        FATAL
    };

    std::string LeveltoStr(LogLevel level)
    {
        switch (level)
        {
        case LogLevel::DEBUG:
            return "DEBUG";
        case LogLevel::INFO:
            return "INFO";
        case LogLevel::WARNING:
            return "WARNING";
        case LogLevel::ERROR:
            return "ERROR";
        case LogLevel::FATAL:
            return "FATAL";
        default:
            return "UNKNOWN";
        }
    }

    std::string GetTimeStamp()
    {
        time_t curr = time(nullptr);
        struct tm curr_tm;
        localtime_r(&curr, &curr_tm);

        char timebuffer[128];
        snprintf(timebuffer, sizeof(timebuffer), "%4d-%2d-%2d  %2d:%2d:%2d"
        , curr_tm.tm_year + 1990
        , curr_tm.tm_mon + 1
        , curr_tm.tm_mday
        , curr_tm.tm_hour
        , curr_tm.tm_min
        , curr_tm.tm_sec
    );

        return timebuffer;
    }

    class Logger
    {
    public:
        Logger()
        {
            EnableConsoleStrategy();
        }
        void EnableFileStrategy()
        {
            _fflush_strategy = std::make_unique<FileLogStrategy>();
        }
        void EnableConsoleStrategy()
        {
            _fflush_strategy = std::make_unique<ConsoleLogStrategy>();
        }

        class LogMessage
        {
        public:
            LogMessage(LogLevel &level, std::string src_name, int line_number)
                : _level(level)
                , _curr_time(GetTimeStamp())
                ,_pid(getpid())
                ,_src_name(src_name)
                ,_line_number(line_number)
                ,_logger(logger)
            {
                std::stringstream ss;
                ss << "[" << _curr_time << "]"
                   << "[" << LeveltoStr(level) << "]"
                   << "[" << _pid << "]"
                   << "[" << _src_name << "]"
                   << "[" << _line_number << "]"
                   << " - ";
                _loginfo = ss.str();
            }

            template <typename T>
            LogMessage &operator<<(const T &info)
            {
                std::stringstream ss;
                ss << info;
                _loginfo += ss.str();
                return *this;
            }

            ~LogMessage()
            {
                if(_logger._fflush_strategy)
                {
                    _logger._fflush_strategy->SyncLog(_loginfo);
                }
            }

        private:
            LogLevel _level;
            std::string _curr_time;
            pid_t _pid;
            std::string _src_name;
            int _line_number;
            std::string _loginfo; // 合并之后，一条完整的信息
            Logger &_logger;
        };

        //
        LogMessage operator()(LogLevel level, std::string name, int line)
        {
            return LogMessage(level, name, line, *this);
        }
        ~Logger()
        {
        }

    private:
        std::unique_ptr<LogStrategy> _fflush_strategy;
    };

    Logger logger;

}