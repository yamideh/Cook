#include "acceptor.h"
#include "EventLoop.h"
#include <string.h>
#include <unistd.h>
#include <iostream>

namespace Cook
{
    Acceptor::Acceptor(EventLoop& loop,const sockaddr_in& listen_addr)
        :loop_(loop)
        ,channel_(loop,Channel::CreateSocketNonBlock())
    {
        channel_.SetReadCallback(std::bind(&Acceptor::HandleRead,this));
        int flag = 1;
        if (-1 == setsockopt(channel_.GetFd(), SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag))) 
        {
            std::cout << " set socket reuse addr failed " << std::endl;
        }
        bind(channel_.GetFd(),(const sockaddr*)&listen_addr,sizeof listen_addr);
    }

    Acceptor::~Acceptor()
    {
        
    }

    void Acceptor::HandleRead()
    {
        struct sockaddr_in addr;
        memset(&addr,0,sizeof(addr));
        socklen_t len = sizeof addr;
        while(true)
        {
            int connfd = accept(channel_.GetFd(), (sockaddr*)&addr,&len);
            if (connfd > 0)
            {
                if(new_connection_callback_)
                {
                    new_connection_callback_(connfd);
                }
                else
                {
                    close(connfd);
                }
            }
        }
    }

    void Acceptor::Listen()
    {
        if((listen(channel_.GetFd(),1000) < 0))
        {
            std::cout << " listen failed " << std::endl;
        }
        channel_.AddToPoller();
    }
}