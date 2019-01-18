#include "LogBuffer.h"
#include <cassert>

namespace CookUtil
{
    LogBuffer::LogBuffer(int32_t init_size)
    {
        assert(init_size > 1);
        all_log_.resize(init_size);
    }

    void LogBuffer::AddLog(const std::string& log);
    {
        assert(log.size() < 1 << 7);
        if( write_index_ - read_index_ + log.size() >= all_log_.size())
        {
            HotExpand(all_log_.size() * 2);
        }
        if( write_index_ % all_log_.size() + log.size < all_log_.size())
        {
            memmove(&all_log_[write_index_ + 1], log.c_str(), log.size());
        }
        else
        {

        }
        IncrWriteIndex(log.size());
    }

    void HotExpand(int32_t size)
    {
        assert( size > init_size);
        std::vector<char> new_log;
        new_log.resize(size);
        {
            std::lock_guard<std::mutex> lock(mutex_);
            for(uint32_t i = read_index_ + 1; i < write_index_ ; ++i)
            {
                memmove(&new_log[0],&all_log_[read_index_],write_index_- read_index_);
            }
            write_index_ -= read_index_;
            read_index_ = 0;
            new_log.swap(all_log_);
        }
    }
}