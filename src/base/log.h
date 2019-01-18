#pragma once

#include <cstdint>
#include <vector>
#include <thread>

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
		    ss << t;
		    return *this;
	    }

        LogMgr& operator << (LogEnd)
        {
            Flush();
            return *this;
        }

        void Flush();

    private:
            
        struct ThreadPair
        {
            int32_t index{ 0 };
            std::thread::id thread_id_{ 0 };
        };

        thread_local std::stringstream ss;
        thread_local LogLevel level_{LogTrace};
        std::vector<ThreadPair> id_2_index_;
        std::vector<LogBuffer> log_buffer_;
    };
}