#pragma once

#include <atomic>
#include <cstdint>

namespace CookUtil
{
    class LogBuffer
    {

    private:
        std::atomic<uint64_t> read_index_{0};
        std::atomic<uint64_t> write_index_{0};
    };
}