#include "log.h"
#include <stdio.h>

#include <cassert>

namespace CookUtil
{
    int32_t LogMgr::thread_num_ = 1;

    LogMgr::LogMgr(int32_t thread_num)
    {
        log_buffer_.resize(thread_num);
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
        const auto index = GetThreadLocalIndex();
        log_buffer_[index].AddLog(sss[index].str());
        sss[index].str("");
    }

    int32_t LogMgr::GetThreadLocalIndex()
    {
        static const thread_local auto current_id = std::this_thread::get_id();     
        static const thread_local auto it = std::find_if(id_2_index_.begin(),id_2_index_.end(),[](const auto& p)->bool  
        {       
            return p.thread_id_ == current_id;  
        });     
        assert( it!= id_2_index_.end());    
        assert( it->index < log_buffer_.size());     
        return it->index;
    }

    LogMgr& LogMgr::operator << (LogEnd)
    {
        sss[GetThreadLocalIndex()] << "\n";
        Flush();
        return *this;
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
                const auto log_size = buf.GetAllLog().size();
                const auto read_pos = buf.GetReadIndex() % log_size;
                const auto write_pos = buf.GetWriteIndex() % log_size;
                if(read_pos == write_pos)
                {
                    continue;
                }
                if(write_pos > read_pos)
                {
                    fwrite(&buf.GetAllLog()[read_pos],sizeof (char),write_pos - read_pos,fp);
                }
                else
                {
                    fwrite(&buf.GetAllLog()[read_pos],sizeof (char),log_size - read_pos,fp);
                    fwrite(&buf.GetAllLog()[0],sizeof (char),write_pos,fp);
                }
                fflush(fp);
                buf.Unlock();
            }
        }
        fclose(fp);
    }
}