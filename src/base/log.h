#pragma once

#include <cstdint>
#include <vector>
#include <thread>
#include <sstream>
#include "LogBuffer.h"
#include <algorithm>

enum LogLevel
{
    LogTrace = 0,
    LogDebug = 1,
    LogWarn = 2,
    LogError = 3,
    LogFatal = 4, 
};

struct LogEnd
{
};

namespace CookUtil
{
    class LogMgr
    {
    public:
        static LogMgr& Instance()
        {
            static LogMgr m;
            return m;
        }

        LogMgr(int32_t thread_num = 1);

        void Register(int32_t thread_index);

        LogMgr& Stream(LogLevel l);

        template <typename T>
	    LogMgr& operator << (const T &t)
	    {
            static const thread_local auto current_id = std::this_thread::get_id();
            static const thread_local auto it = std::find_if(id_2_index_.begin(),id_2_index_.end(),[](const auto& p)->bool
            {
                return p.thread_id_ == current_id;
            });
		    sss[it->index] << t;
		    return *this;
	    }

        LogMgr& operator << (LogEnd)
        {
            Flush();
            return *this;
        }

        void Loop();

        void Flush();

    private:
        struct ThreadPair
        {
            int32_t index{ 0 };
            std::thread::id thread_id_{ 0 };
        };

        std::vector<std::stringstream> sss;
        std::vector<ThreadPair> id_2_index_;
        std::vector<LogBuffer> log_buffer_;
    };
}