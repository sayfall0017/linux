// 翻译单词、录入词典文件
#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include "Inetaddr.hpp"

const std::string defaultdict = "./dictionary.txt";
const std::string sep = ": ";

class Dict
{
public:
    Dict(const std::string &path = defaultdict) : _pathfile(path)
    {
    }
    bool LoadDict()
    {
        std::ifstream in(_pathfile);
        if(!in.is_open())
        {
            LOG(LogLevel::FATAL) << "配置失败...";
            return false;
        }
        std::string line;
        while(getline(in, line))
        {
            int pos = line.find(sep);
            if(pos == std::string::npos)
            {
                LOG(LogLevel::WARNING) << "格式错误：" << line;
                continue;
            }
            std::string english = line.substr(0, pos);
            std::string chinese = line.substr(pos + sep.size());
            if(chinese.empty() || english.empty())
            {
                LOG(LogLevel::WARNING) << "格式错误：" << line;
                continue;
            }
            _dict.insert(std::make_pair(english, chinese));
            LOG(LogLevel::DEBUG) << "加载成功： " << line;
        }

        LOG(LogLevel::INFO) << "配置完成！";
        in.close();
        return true;
    }

    std::string Translate(const std::string &word, InetAddr &client)
    {
        auto iter = _dict.find(word);

        if(iter == _dict.end())
        {
            LOG(LogLevel::DEBUG) << "进入到了翻译模块, [" << client.Ip() << " : " << client.Port() << "]# " << word << "不存在";
            std::string result = word + "不存在";
            return result;
        }
        LOG(LogLevel::INFO) << "进入到了翻译模块, [" << client.Ip() << " : " << client.Port() << "]# " << word << iter->second;
        return iter->second;
    }

    ~Dict()
    {
    }

private:
    std::string _pathfile;
    std::unordered_map<std::string, std::string> _dict;
};

