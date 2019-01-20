#pragma once

namespace CookUtil
{
    class WorkerThread;

    class LogThread
    {

    public:
        void Loop();

        LogThread();
        ~LogThread();

    private:
        WorkerThread* t_{ nullptr };
    };
}