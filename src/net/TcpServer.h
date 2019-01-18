#pragma once

#include <unordered_map>
#include "acceptor.h"
#include "EventLoop.h"
#include <thread>
#include <vector>


namespace Cook
{
    class TcpConn;

    class Worker
    {
        friend class TcpServer;
    public:
        Worker();
        ~Worker();
    private:
        std::thread* t;
        EventLoop* loop;
    };

    class TcpServer
    {
    public:

        TcpServer(const sockaddr_in& listen_addr,int thread_num = 1);

        static uint64_t GetSessionID(uint32_t);

        void OnNewConnection(int fd);
        void OnConnectionDisconnected(const ConnPtr& ptr);

        EventLoop& GetNextLoop();

        void Start();

    private:
        static uint64_t acceptor_index;
        std::unordered_map<uint64_t,ConnPtr> all_conns_;
        EventLoop main_loop_;
        Acceptor acceptor_;
        std::vector<WorkerThread> thread_pool_;
        std::vector<EventLoop> loop_pool_;
        WorkerThread* log_thread_;
    };
}