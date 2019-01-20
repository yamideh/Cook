#pragma once

#include <cstdint>
#include <vector>
#include "MutexWapper.h"

namespace CookUtil
{
    class LogBuffer
    {
    public:
        LogBuffer(int32_t init_size = (1 << 7));
        LogBuffer(LogBuffer&& ) = delete;
        ~LogBuffer();

        void AddLog(const std::string& log); 
        uint64_t GetWriteIndex() const { return write_index_ ;}
        uint64_t GetReadIndex() const { return read_index_;}

        void IncrWriteIndex(uint32_t n) { write_index_ += n; }
        void IncrReadIndex(uint32_t n) { read_index_ += n;}

        //目前只支持扩容
        void HotExpand(int32_t size);
    
        void Lock(){ expand_mutex_.lock();}
        void Unlock() { expand_mutex_.unlock();}

        const std::vector<char>& GetAllLog() const { return all_log_;}

    private:
        std::vector<char> all_log_;
        uint64_t read_index_{0};
        uint64_t write_index_{0};
        MutexWrapper expand_mutex_;
    };
}