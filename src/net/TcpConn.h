#pragma once

#include <cstdint>
#include <string>
#include "CallbackDef.h"
#include "Buffer.h"
#include "Channel.h"


namespace Cook
{
    enum ConnectState
    {
        kConnectStateConnectting,
        kConnectStateConnectted,
        kConnectStateDisconnectting,
        kConnectStateDisconnectted,
    };

    class EventLoop;

    void DefaultEchoMessageCallback(const ConnPtr& ptr,Buffer* buf);

    class TcpConn : public std::enable_shared_from_this<TcpConn>
    {
    public:

        TcpConn(uint64_t sid,EventLoop& l,int32_t sock_fd);
        ~TcpConn();

        void Send(const char* buf, std::size_t size);
        void Send(const std::string& str);

        void SendInLoop(const char* buf, std::size_t size);

        void HandleRead();
        void HandleWrite();
        void HandleClose();
        
        void OnConnectionBuild();

        void SetMessageCallback(const MessageCallback& cb)
        {
            message_callback_ = cb;
        }

        void SetWriteCompleteCallback(const WriteCompleteCallback& cb)
        {
            write_complete_callback_ = cb;
        }

        void SetCloseCallback(const CloseCallback& cb)
        {
            close_callback_ = cb;
        }
        
        void SetState(ConnectState s)
        {
            state_ = s;
        }

        EventLoop& GetLoop(){ return loop_;}
        int32_t GetFd() const { return fd_;}
        uint64_t GetSessionID() const{ return session_id_;}

    private:
        uint64_t session_id_{0};
        int32_t fd_{0};
        EventLoop& loop_;
        ConnectState state_{ kConnectStateDisconnectted };
        MessageCallback message_callback_{ nullptr };
        WriteCompleteCallback write_complete_callback_{ nullptr };
        ConnectionCallback connection_callback_{ nullptr };
        CloseCallback close_callback_{ nullptr };
        Buffer read_buffer_;
        Buffer write_buffer_;
        Channel channel_;
    };
}
