#include "log.h"
#include <stdio.h>

#include <cassert>

namespace CookUtil
{
    LogMgr::LogMgr(int32_t thread_num)
    {
        // while(thread_num -- >0)
        // {
        //     log_buffer_.emplace_back();
        // }
        // log_buffer_.resize(thread_num);
    }

    void LogMgr::Register(int32_t thread_index)
    {
        id_2_index_.emplace_back();
        auto& p = id_2_index_.back();
        p.thread_id_ = std::this_thread::get_id();
        p.index = thread_index;
    }

    LogMgr& LogMgr::Stream(LogLevel level)
    {
        // level_ = level;
    }

    void LogMgr::Flush()
    {
        static const thread_local auto current_id = std::this_thread::get_id();
        static const thread_local auto it = std::find_if(id_2_index_.begin(),id_2_index_.end(),[](const auto& p)->bool
        {
            return p.thread_id_ == current_id;
        });
        static const thread_local auto index = it->index;
        assert( it!= id_2_index_.end());
        assert(index < log_buffer_.size());
        log_buffer_[index].AddLog(sss[index].str());
        sss[index].str("");
    }

    void LogMgr::Loop()
    {
        auto fp = fopen("/tmp/log/123","a");
        assert(fp);
        for(;;)
        {
            for(auto& buf : log_buffer_)
            {
                buf.Lock();
                const auto write_index = buf.GetWriteIndex();
                const auto log_size = buf.GetAllLog().size();
                for(auto i = buf.GetReadIndex(); i <= write_index; ++i)
                {
                    fflush(fp);
                }
                buf.Unlock();
            }
        }
        fclose(fp);
    }
}