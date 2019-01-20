#pragma once
#include <functional>
#include <vector>
#include <thread>
#include "poller.h"
#include <cassert>
#include "acceptor.h"
#include <mutex>
#include "MutexWapper.h"

namespace Cook
{
    class EventLoop
    {
    public:
        EventLoop();
        ~EventLoop();

        using PendingFunctor = std::function<void()>;
        void RunInloop(const PendingFunctor& f);

        bool IsInLoopThread() const;

        void Loop();

        void AssertInLoopThread()
        {
            assert(thread_id_ == std::this_thread::get_id());
        }

        Poller& GetPoller(){ return poller_;}

        void DoPendingFunctors();
        void Init();

    private:
        std::vector<Channel*> active_channels_;
        std::thread::id thread_id_{ 0 };
        std::vector<PendingFunctor> pending_functor_;
        Poller poller_;
        MutexWrapper mutex_; 
        uint64_t times_{ 0};
    };
}
