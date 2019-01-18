#include "LogBuffer.h"
#include <cassert>

namespace CookUtil
{
    LogBuffer::LogBuffer(int32_t init_size)
    {
        assert(init_size > 1);
        all_log_.resize(init_size);
    }

    void LogBuffer::AddLog(std::string&& log);
    {
        if( write_index_ - read_index_ >= all_log_.size() )
        {
            HotExpand(all_log_.size() * 2);
        }
        all_log_[write_index_ % all_log_.size()] = std::move(log);
        IncrWriteIndex();
    }

    void HotExpand(int32_t size)
    {
        assert( size > init_size);
        std::vector<std::string> new_log_;
        new_log.resize(size);
        uint32_t j = 0;
        {
            std::lock_guard<std::mutex> lock(mutex_);
            for(uint32_t i = read_index_ + 1; i < write_index_ ; ++i)
            {
                new_log_[j++] = std::move(all_log_[i]);
            }
            read_index_ = 0;
            write_index_ = j - 1;
            new_log_.swap(all_log_);
        }
    }
}