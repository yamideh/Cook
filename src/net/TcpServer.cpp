#include "TcpServer.h"
#include "TcpConn.h"
#include <iostream>
#include <sys/socket.h>

namespace Cook
{
    WorkerThread::WorkerThread()
    {
        loop = new EventLoop;
        t = new std::thread(std::bind(&EventLoop::Loop,loop));
    }

    WorkerThread::~WorkerThread()
    {
        t->join();
        delete loop;
        delete t;
    }

    uint64_t TcpServer::acceptor_index = 0;

    TcpServer::TcpServer(const sockaddr_in& listen_addr,int thread_num)
        :acceptor_(main_loop_,listen_addr)
    {
        main_loop_.Init();
        assert(thread_num > 0);
        acceptor_.SetNewConnectionBack(std::bind(&TcpServer::OnNewConnection,this,std::placeholders::_1));
        thread_pool_.resize(thread_num);
    }

    uint64_t TcpServer::GetSessionID(uint32_t fd)
    {
        return (acceptor_index << 24) | fd;
    }

    void TcpServer::OnNewConnection(int fd)
    {
        auto& next_loop = GetNextLoop();
        const auto session_id = GetSessionID(fd);
        ConnPtr new_conn(new TcpConn(session_id,next_loop,fd));

        new_conn->SetCloseCallback(std::bind(&TcpServer::OnConnectionDisconnected,this,std::placeholders::_1));
        new_conn->SetMessageCallback(std::bind(DefaultEchoMessageCallback,std::placeholders::_1,std::placeholders::_2));

        all_conns_[session_id] = new_conn;
        
        sockaddr_in peer_info;
        socklen_t info_size = sizeof peer_info;
        getpeername(fd,(sockaddr*)&peer_info,&info_size);

        std::cout << " session " << session_id << " connected peer ip is "<< inet_ntoa(peer_info.sin_addr) << " port is "
            << ntohs(peer_info.sin_port) << std::endl;
        next_loop.RunInloop(std::bind(&TcpConn::OnConnectionBuild,new_conn));
    }

    void TcpServer::OnConnectionDisconnected(const ConnPtr& ptr)
    {
        all_conns_.erase(ptr->GetSessionID());
        std::cout << " session " << ptr->GetSessionID() << " disconnected " << std::endl;
    }

    EventLoop& TcpServer::GetNextLoop()
    {
        return *thread_pool_[(acceptor_index++)%thread_pool_.size()].loop;
    }

    void TcpServer::Start()
    {
        acceptor_.Listen();
        main_loop_.Loop();
    }
}