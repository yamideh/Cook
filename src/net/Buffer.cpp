#include "Buffer.h"
#include <cassert>
#include <sys/uio.h>
#include <unistd.h>
#include <iostream>

namespace Cook
{
    Buffer::Buffer()
    {
        data_.reserve(default_msg_size);
    }

    Buffer::~Buffer()
    {
        write_index_ = read_index_ = 0;
    }

    void Buffer::Append(const char * buf, uint32_t size)
    {
        if (size == 0)
        {
            return;
        }
        if (size + write_index_ > data_.capacity())
        {
            data_.reserve(size + write_index_);
        }
        std::copy(buf, buf + size, GetWritePos());
        write_index_ += size;
    }

    void Buffer::Append(const std::string & str)
    {
        Append(str.data(), str.size());
    }

    void Buffer::Retrieve(uint32_t size)
    {
        assert(size <= write_index_ - read_index_);
        read_index_ += size;
        if(read_index_ == write_index_)
        {
            read_index_ = write_index_ = 0;
        }
    }

    void Buffer::RetrieveAll()
    {
        read_index_ = write_index_ = 0;
    }

    std::string Buffer::RetrieveAsString(uint32_t size)
    {
        std::string result(GetReadPos(), size);
        Retrieve(size);
        return result;
    }

    std::string Buffer::RetrieveAllAsString()
    {
        std::string result(GetReadPos(), write_index_ - read_index_);
        RetrieveAll();
        return result;
    }

    char * Buffer::GetWritePos()
    {
        return &data_[0] + write_index_;
    }
    const char * Buffer::GetReadPos() const
    {
        return &data_[0] + read_index_;
    }

    uint32_t Buffer::ReadFd(uint32_t fd)
    {
        char extrabuf[65535];
        struct iovec vec[2];
        const auto writeable = WriteableBytes();
        vec[0].iov_base = GetWritePos();
        vec[0].iov_len = writeable;
        vec[1].iov_base = extrabuf;
        vec[1].iov_len = sizeof writeable;

        const auto n = readv(fd, vec, 2);
        if(n < 0)
        {
            std::cout << " errno is " << errno << std::endl;
            return 0;
        }
        if( n<= writeable)
        {
            write_index_ += n;
        }
        else
        {
            write_index_ = data_.size();
            Append(extrabuf, n - writeable);
        }
        return n;
    }
    uint32_t Buffer::WriteFd(uint32_t fd)
    {
        const auto n = ::write(fd, GetReadPos(), ReadableBytes());
        Retrieve(n);
        return n;
    }
}


