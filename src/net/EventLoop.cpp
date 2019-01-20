#include "EventLoop.h"
#include <thread>

namespace Cook
{
    EventLoop::EventLoop()
    {
    }

    EventLoop::~EventLoop()
    {
    }

    void EventLoop::RunInloop(const PendingFunctor & f)
    {
        pending_functor_.emplace_back(f);
    }

    bool EventLoop::IsInLoopThread() const
    {
        return std::this_thread::get_id() == thread_id_;
    }

    void EventLoop::Init()
    {
        thread_id_ = std::this_thread::get_id(); 
    }

    void EventLoop::Loop()
    {
        Init();

        while(true)
        {
            times_++;
            active_channels_.clear();
            poller_.Poll(1000,&active_channels_);
            for(auto c : active_channels_)
            {
                c->HandleEvent();
            }
            DoPendingFunctors();
        }
    }

    void EventLoop::DoPendingFunctors()
    {
        std::vector<PendingFunctor> functors;
        {
            std::lock_guard<MutexWrapper> lock(mutex_);
            functors.swap(pending_functor_);
        }
        for(auto& func : functors)
        {
            func();
        }
    }
    
}
