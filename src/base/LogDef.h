#include "log.h"

#define LOGTRACE CookUtil::LogMgr::Instance().Stream(LogTrace) << " [TRACE] "  << __FUNCTION__ << "(" << __FILE__ << ':' << __LINE__ << ") "
#define LOGDEBUG CookUtil::LogMgr::Instance().Stream(LogDebug) << " [DEBUG] "  << __FUNCTION__ << "(" << __FILE__ << ':' << __LINE__ << ") " 
#define LOGWARN CookUtil::LogMgr::Instance().Stream(LogWarn) << " [WARN] "  << __FUNCTION__ << "(" << __FILE__ << ':' << __LINE__ << ") "
#define LOGERROR CookUtil::LogMgr::Instance().Stream(LogError) << " [ERROR] " << __FUNCTION__ << "(" << __FILE__ << ':' << __LINE__ << ") " 
#define LOGFATAL CookUtil::LogMgr::Instance().Stream(LogFatal) << " [FATAL] " << __FUNCTION__ << "(" << __FILE__ << ':' << __LINE__ << ") "

#define LOGEND CookUtil::LogEnd()
