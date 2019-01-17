#include "TcpConn.h"
#include "EventLoop.h"
#include <unistd.h>
#include <iostream>
#include <chrono>
namespace Cook
{
    void DefaultEchoMessageCallback(const ConnPtr& ptr,Buffer* buf)
    {
        std::string msg(buf->GetReadPos(),buf->ReadableBytes());
        ptr->Send(buf->GetReadPos(),buf->ReadableBytes());
        buf->RetrieveAll();
    }

    TcpConn::TcpConn(uint64_t sid,EventLoop& l,int32_t sock_fd)
        :session_id_(sid)
        ,fd_(sock_fd)
        ,loop_(l)
        ,channel_(l,sock_fd)
    {
        channel_.SetReadCallback(std::bind(&TcpConn::HandleRead,this));
        channel_.SetWriteCallback(std::bind(&TcpConn::HandleWrite,this));
    }

    TcpConn::~TcpConn()
    {
        close(fd_);
    }

    void TcpConn::Send(const char * buf, std::size_t size)
    {
        if( state_ != kConnectStateConnectted)
        {
            return;
        }
        if(loop_.IsInLoopThread())
        {
            SendInLoop(buf, size);
        }
        else
        {
            loop_.RunInloop(std::bind(&TcpConn::SendInLoop, this, buf, size));
        }
    }

    void TcpConn::Send(const std::string & str)
    {
        Send(str.data(), str.size());
    }

    void TcpConn::SendInLoop(const char * buf, std::size_t size)
    {
        write_buffer_.Append(buf, size);
        channel_.EnableWriting();
    }

    void TcpConn::HandleRead()
    {
        const auto n = read_buffer_.ReadFd(fd_);
        if( n > 0)
        {
            message_callback_(shared_from_this(), &read_buffer_);
        }
        if( n == 0 )
        {
            HandleClose();
        }
    }
    void TcpConn::HandleWrite()
    {
        loop_.AssertInLoopThread();
        const auto n = write_buffer_.WriteFd(fd_);
        if( n > 0)
        {
            if(write_buffer_.ReadableBytes() == 0)
            {
                //todo 
                if (write_complete_callback_)
                {
                    
                }
                channel_.DisableWriting();
            }
        }
        else
        {
            HandleClose();
        }
    }

    void TcpConn::OnConnectionBuild()
    {
        SetState(kConnectStateConnectted);
        channel_.Bind(shared_from_this());
        channel_.AddToPoller();
    }

    void TcpConn::HandleClose()
    {
        channel_.RemoveToPoller();
        ConnPtr guard_this(shared_from_this());
        close_callback_(guard_this);
    }
}


