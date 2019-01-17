#pragma once

#include <vector>
#include <cstdint>
#include <string>

namespace Cook
{
    constexpr int32_t default_msg_size = 4 * 1024; 

    class Buffer
    {
    public:
        Buffer();
        ~Buffer();

        uint32_t ReadableBytes() const { return write_index_ - read_index_; }
        uint32_t WriteableBytes() const { return data_.capacity() - write_index_; }

        void Append(const char* buf, uint32_t size);
        void Append(const std::string& str);

        void Retrieve(uint32_t size);
        void RetrieveAll();

        std::string RetrieveAsString(uint32_t);
        std::string RetrieveAllAsString();

        char* GetWritePos();
        const char* GetReadPos() const;

        uint32_t ReadFd(uint32_t fd);
        uint32_t WriteFd(uint32_t fd);
        
    private:
        std::vector<char> data_;
        uint32_t read_index_{ 0 };
        uint32_t write_index_{ 0 };
    };
}
