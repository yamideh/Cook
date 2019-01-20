#include "poller.h"
#include <unistd.h>
#include <string.h>
#include "Channel.h"
#include <iostream>
#include <stdio.h>
#include <cassert>
#include <thread>
#include <chrono>
#include "LogDef.h"

namespace Cook
{
    Poller::Poller()
    {
        poll_fd_ = epoll_create1(EPOLL_CLOEXEC);
        all_events_.resize(16);
        LOGDEBUG << " fd is " << poll_fd_ << LOGEND;
    }

    Poller::~Poller()
    {
        close(poll_fd_);
    }
            
    void Poller::AddChannel(Channel* channel)
    {
        epoll_event event;
        memset(&event,sizeof event,0);
        event.events = EPOLLIN;
        event.data.ptr = channel;
        auto fd = Channel::CreateSocketNonBlock();
        auto result = epoll_ctl(poll_fd_,EPOLL_CTL_ADD,channel->GetFd(),&event);
        if(result == -1)
        {
            perror("fsdfewfw");
            std::cout << " errno is " << errno <<std::endl;
        }
    }
    
    void Poller::UpdateChannel(Channel* channel)
    {
        epoll_event event;
        memset(&event,sizeof event,0);
        event.events = channel->GetFlag();
        event.data.ptr = channel;
        auto result = epoll_ctl(poll_fd_,EPOLL_CTL_MOD,channel->GetFd(),&event);
        if(result == -1)
        {
            std::cout << " errno is " << errno << " line is "<< __LINE__ <<std::endl;
        }
    }
    
    void Poller::RemoveChannel(Channel* channel)
    {
        epoll_event event;
        epoll_ctl(poll_fd_,EPOLL_CTL_DEL,channel->GetFd(),&event);
    }

    int32_t Poller::Poll(int32_t millseconds,std::vector<Channel*>* active_channels)
    {
        auto event_num = epoll_wait(poll_fd_,&(*all_events_.begin()),static_cast<int32_t>(all_events_.size()),millseconds);
        if(event_num <0)
        {
            std::cout << " epoll wrong errno is " << errno << std::endl;
            return 0;
        }
        if(event_num == 0)
        {
            return 0;
        }
        auto m = std::min(event_num,static_cast<int32_t>(all_events_.size()));
        active_channels->clear();
        for(int i = 0 ; i < m ; ++i)
        {
            auto c = static_cast<Channel*>(all_events_[i].data.ptr);
            c->SetEvent(all_events_[i].events);
            active_channels->emplace_back(c);
        }
        if( static_cast<int32_t>(all_events_.size()) == event_num)
        {
            all_events_.resize(all_events_.size()*2);
        }
        return event_num;
    }
}