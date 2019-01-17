#pragma once

#include "CallbackDef.h"

namespace Cook
{
    class EventLoop;

    
    class Channel
    {
    public:
        Channel(EventLoop& loop,int32_t sock_fd);
        ~Channel();

        static int32_t CreateSocketNonBlock();

        void AddToPoller();
        void UpdateToPoller();
        void RemoveToPoller();

        void Bind(const std::shared_ptr<void>& ptr);
        
        void SetReadCallback(const ReadEventCallback& cb)
        {
            read_callback_ = cb;
        }
        void SetWriteCallback(const WriteEventCallback& cb)
        {
            write_callback_ = cb;
        }

        void EnableWriting();
        void DisableWriting();

        void HandleEvent();

        int32_t GetFd() const { return fd_;}
        int32_t GetFlag() const { return flag_;}
        void SetEvent(int32_t e) { event_ = e;}
    private:
        int32_t fd_{0};
        int32_t flag_;
        int32_t event_{0};
        std::weak_ptr<void> conn_ptr_;
        bool tied_{false};
        EventLoop& loop_;
        ReadEventCallback read_callback_;
        WriteEventCallback write_callback_;
        
    };
}