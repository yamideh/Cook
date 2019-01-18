#include "log.h"

namespace CookUtil
{
    LogMgr::LogMgr(int32_t thread_num)
    {
        log_buffer_.resize(thread_num);
    }

    void LogMgr::Register(int32_t thread_index)
    {
        ThreadPair p { thread_index, std::this_thread::get_id()};
        id_2_index_.emplace_back(p);
    }

    LogMgr& LogMgr::Stream(LogLevel level)
    {
        level_ = level;
    }

    void LogMgr::Flush()
    {
        static thread_local current_id = std::this_thread::get_id();
        auto it = std::find_if(id_2_index_.begin(),id_2_index_.end(),[current_id](const auto& p)->bool
        {
            return p.thread_id_ == current_id;
        });
        assert( it!= id_2_index_.end());
        assert(it.index < log_buffer_.size());
        log_buffer_[it.index].AddLog(ss.str());
        ss.str("");
    }
}