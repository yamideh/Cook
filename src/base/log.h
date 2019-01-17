#pragma once

#include <cstdint>
#include <vector>
#include <thread>

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

        struct ThreadPair
        {
            int32_t index{0};
            std::thread::id thread_id_{ 0 };
        };

        void Register(int32_t thread_index);

    private:
        std::vector<ThreadPair> id_2_index_;
        // std::vector<RingBuffer> log_buffer_;
    };
}