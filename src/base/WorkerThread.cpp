#include "WorkerThread.h"
#include "log.h"

namespace CookUtil
{
    WorkerThread::WorkerThread(const std::function<void()>& func)
        : func_(func)
    {
        static int32_t i_index = 1;
        thread_index_ = i_index ++;
        thread_ = new std::thread(&WorkerThread::Start,this);
    }

    WorkerThread::~WorkerThread()
    {
        thread_->join();
        delete thread_;
    }

    void WorkerThread::Start()
    {
        LogMgr::Instance().Register(thread_index_);
        func_();
    }
}