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

namespace CookUtil
{
    struct LogEnd
    {
    };

    class LogMgr
    {
    public:
        static LogMgr& Instance()
        {
            static auto m = new LogMgr(thread_num_);
            return *m;
        }

        LogMgr(int32_t thread_num = 1);

        void Register(int32_t thread_index);

        LogMgr& Stream(LogLevel l);

        int32_t GetThreadLocalIndex();

        template <typename T>
	    LogMgr& operator << (const T &t)
	    {
            sss[GetThreadLocalIndex()]<< t;
		    return *this;
	    }

        LogMgr& operator << (LogEnd);

        void Loop();

        void Flush();

        static void SetThreadNum(int32_t num) { thread_num_ = num ;}
    private:
        struct ThreadPair
        {
            int32_t index{ 0 };
            std::thread::id thread_id_{ 0 };
        };
        static int32_t thread_num_;
        std::vector<std::stringstream> sss;
        std::vector<ThreadPair> id_2_index_;
        std::vector<LogBuffer> log_buffer_;
    };
}