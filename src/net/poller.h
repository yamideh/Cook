#pragma once

#include <cstdint>
#include <vector>
#include <sys/epoll.h>

namespace Cook
{
    class Channel;

    class Poller
    {
    public:
        Poller();
        ~Poller();
        void AddChannel(Channel*);
        void UpdateChannel(Channel*);
        void RemoveChannel(Channel*);

        int32_t Poll(int32_t millseconds,std::vector<Channel*>* active_channels); 
        
    private:
        uint32_t poll_fd_{0};
        std::vector<epoll_event> all_events_;
    };
}