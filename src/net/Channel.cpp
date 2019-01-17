#include "Channel.h"
#include "poller.h"
#include "EventLoop.h"
#include <sys/epoll.h>
#include <sys/socket.h>
#include <iostream>
namespace Cook
{
    static const int kNoneEvent = 0 ;
    static const int kReadEvent = EPOLLIN | EPOLLPRI;
    static const int kWriteEvent = EPOLLOUT ;

    int32_t Channel::CreateSocketNonBlock()
    {
        return socket(AF_INET,SOCK_STREAM| SOCK_NONBLOCK | SOCK_CLOEXEC,IPPROTO_TCP);
    }

    Channel::Channel(EventLoop& loop,int32_t sock_fd)
        : loop_(loop)
        , flag_(kReadEvent)
        , fd_(sock_fd)
    {
    }

    Channel::~Channel()
    {
    }

    void Channel::AddToPoller()
    {
        loop_.AssertInLoopThread();
        loop_.GetPoller().AddChannel(this);
        UpdateToPoller();
    }

    void Channel::UpdateToPoller()
    {
        loop_.AssertInLoopThread();
        loop_.GetPoller().UpdateChannel(this);
    }

    void Channel::RemoveToPoller()
    {
        loop_.AssertInLoopThread();
        loop_.GetPoller().RemoveChannel(this);
    }

    void Channel::EnableWriting()
    {
        loop_.AssertInLoopThread();
        flag_ |= kWriteEvent;
        UpdateToPoller();
    }

    void Channel::DisableWriting()
    {
        loop_.AssertInLoopThread();
        flag_ &= ~kWriteEvent;
        UpdateToPoller();
    }

    void Channel::Bind(const std::shared_ptr<void>& ptr)
    {
        conn_ptr_ = ptr;
        tied_ = true;
    }

    void Channel::HandleEvent()
    {
        loop_.AssertInLoopThread();
        auto lock = conn_ptr_.lock();
        if(tied_ 
            && !lock)
        {
            return;
        }
        if (event_ & (EPOLLIN | EPOLLPRI))
        {
            if (read_callback_) read_callback_();
        }
        if (event_ & EPOLLOUT)
        {
            if (write_callback_) write_callback_();
        }
    }
}