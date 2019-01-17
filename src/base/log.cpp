#include "log.h"

namespace CookUtil
{
    void LogMgr::Register(int32_t thread_index)
    {
        ThreadPair p { thread_index, std::this_thread::get_id()};
        id_2_index_.emplace_back(p);
    }

    

}