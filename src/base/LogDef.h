#include "Log.h"

#define LOGTRACE LogMgr::Instance().Stream(LogTrace) << " [TRACE] "  << __FUNCTION__ << "(" << __FILENAME__ << ':' << __LINE__ << ") "
#define LOGDEBUG LogMgr::Instance().Stream(LogDebug) << " [DEBUG] "  << __FUNCTION__ << "(" << __FILENAME__ << ':' << __LINE__ << ") " 
#define LOGWARN LogMgr::Instance().Stream(LogWarn) << " [WARN] "  << __FUNCTION__ << "(" << __FILENAME__ << ':' << __LINE__ << ") "
#define LOGERROR LogMgr::Instance().Stream(LogError) << " [ERROR] " << __FUNCTION__ << "(" << __FILENAME__ << ':' << __LINE__ << ") " 
#define LOGFATAL LogMgr::Instance().Stream(LogFatal) << " [FATAL] " << __FUNCTION__ << "(" << __FILENAME__ << ':' << __LINE__ << ") "

#define LOGEND LogEnd()
