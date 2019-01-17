#pragma once

#include <thread>
#include <functional>

namespace CookUtil
{
    class WorkerThread
    {
    public:
        WorkerThread(const std::function<void()>& func);

        void Start();

    private:
        std::thread* thread_; 
        int32_t thread_index_;
        std::function<void()> func_{nullptr};
    };
}