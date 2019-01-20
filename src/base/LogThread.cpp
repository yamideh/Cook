#include "WorkerThread.h"
#include "LogThread.h"
#include "log.h"
#include <functional>

namespace CookUtil
{
    LogThread::LogThread()
    {
        t_ = new WorkerThread(std::bind(&LogThread::Loop,this));
    }

    LogThread::~LogThread()
    {
        delete t_;
    }

    void LogThread::Loop()
    {
        LogMgr::Instance().Loop();
    }
}