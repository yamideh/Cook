#pragma once

#include <cstdint>
#include <vector>
#include <mutex>

namespace CookUtil
{
    class LogBuffer
    {
    public:
        LogBuffer(int32_t init_size = (1 << 7));

        void AddLog(const std::string& log); 
        uint64_t GetWriteIndex() const { return write_index_ ;}
        uint64_t GetReadIndex() const { return read_index_;}

        void IncrWriteIndex() { ++ write_index_; }
        void IncrReadIndex() { ++ read_index_;}

        //目前只支持扩容
        void HotExpand(int32_t size);
    
        std::mutex& GetLogMutex() { return expand_mutex_;}

        const std::vector<std::string>& GetAllLog() const { return all_log_;}

    private:
        std::vector<std::string> all_log_; 
        std::vector<char> all_log_;
        uint64_t read_index_{0};
        uint64_t write_index_{0};
        std::mutex expand_mutex_;
    };
}