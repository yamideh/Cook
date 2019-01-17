#pragma once

#include "Channel.h"
#include <arpa/inet.h>

namespace Cook
{
    class EventLoop;
    class Acceptor
    {
    public:

        Acceptor(EventLoop& loop,const sockaddr_in& listen_addr);
        ~Acceptor();

        void HandleRead();
        
        void SetNewConnectionBack(const NewConnectionCallback& cb)
        {
            new_connection_callback_ = cb;
        }
        void Listen();

    private:
        Channel channel_;
        EventLoop& loop_;
        NewConnectionCallback new_connection_callback_{nullptr};
    };
}