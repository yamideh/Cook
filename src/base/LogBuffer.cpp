#include "LogBuffer.h"
#include <cassert>
#include <string.h>

namespace CookUtil
{
    LogBuffer::LogBuffer(int32_t init_size)
    {
        assert(init_size > 1);
        all_log_.resize(init_size);
    }

    LogBuffer::~LogBuffer()
    {
    }

    void LogBuffer::AddLog(const std::string& log)
    {
        assert(log.size() < 1 << 7);
        if( write_index_ - read_index_ + log.size() >= all_log_.size())
        {
            HotExpand(all_log_.size() * 2);
        }
        if( write_index_ % all_log_.size() + log.size() < all_log_.size())
        {
            memmove(&all_log_[write_index_ + 1], log.c_str(), log.size());
        }
        else
        {
            const auto left_in_the_end = all_log_.size() - write_index_ % all_log_.size() ;
            memmove(&all_log_[write_index_ + 1], log.c_str(), left_in_the_end);
            memmove(&all_log_[0], log.c_str() + left_in_the_end, log.size() - left_in_the_end);
        }
        IncrWriteIndex(log.size());
    }

    void LogBuffer::HotExpand(int32_t size)
    {
        assert( size > all_log_.size());
        std::vector<char> new_log;
        new_log.resize(size);
        {
            std::lock_guard<std::mutex> lock(expand_mutex_);
            const auto read_pos = read_index_ % all_log_.size();
            const auto write_pos = write_index_ % all_log_.size(); 
            if( read_pos >= write_pos )
            {   
                const auto left_in_the_end = all_log_.size() - read_pos;
                memmove(&new_log[0],&all_log_[read_index_],all_log_.size() - read_pos);
                memmove(&new_log[left_in_the_end],&all_log_[0],write_pos);
            }
            else
            {
                memmove(&new_log[0],&all_log_[read_index_], write_pos - read_pos);
            }

            write_index_ -= read_index_;
            read_index_ = 0;
            new_log.swap(all_log_);
        }
    }
}