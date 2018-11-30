#pragma once
#include <XLN/Core/Config/Config.h>
#include <folly/ThreadLocal.h>

/************************************************************************/
/*Log utility: use XLN_LOG define to output log
     XLN_LOG suffixes with a letter to indicate level of log.
     E - Error
     W - Warning
     D - Debug
     I - Info
     
     Debug and verbose Level log will be disabled under release version
     
     TODO:
        -logger will lock the logging file during execution
        
     
NOTE: try to not include this file in .h files
/************************************************************************/



namespace XCr
{


enum LogLevel
{   LL_LOG_INFO,
    LL_LOG_DEBUG,
    LL_LOG_WARNING,
    LL_LOG_ERROR,
    //LL_LOG_DISABLED,
};


class XLN_CLASS_DECL Log
{
    public:
        Log(const char* pcFormat, ...);
        ~Log(void);
        
        static void SetLogLevel(LogLevel level);
        static bool SetLogInfo(const char* name, const char* funcName, int lineNum);
    
    
};

class XLN_CLASS_DECL LogDebug
{
public:
    LogDebug(const char* pcFormat, ...);
    LogDebug(const char* tag, const char* pcFormat, ...);
};


class XLN_CLASS_DECL LogError
{
    public:
        LogError(const char* pcFormat, ...);
        LogError(const char* tag, const char* pcFormat, ...);
};


class XLN_CLASS_DECL LogWarning
{
    public:
        LogWarning(const char* pcFormat, ...);
        LogWarning(const char* tag, const char* pcFormat, ...);
};

class XLN_CLASS_DECL LogInfo
{
    public:
        LogInfo(const char* pcFormat, ...);
        LogInfo(const char* tag, const char* pcFormat, ...);
};

}

#ifndef NDEBUG
    //private defined

    #define __XLN_LOGI__ XCr::LogInfo
    #define __XLN_LOGD__ XCr::LogDebug

    //////////////////////////////////////////////////////////////////////////

    #define XLN_LOGI XCr::Log::SetLogInfo(__FILE__, __FUNCTION__, __LINE__)?NULL:__XLN_LOGI__
    #define XLN_LOGD XCr::Log::SetLogInfo(__FILE__, __FUNCTION__, __LINE__)?NULL:__XLN_LOGD__
    #define XLN_LOG XCr::Log::SetLogInfo(__FILE__, __FUNCTION__, __LINE__)?NULL:__XLN_LOGD__

#else
    #if defined (_MSC_VER) && (_MSC_VER >= 1300)

        #define XLN_LOGI __noop
        #define XLN_LOGD __noop
        #define XLN_LOG __noop

    #else
        #define XLN_LOGI //
        #define XLN_LOGD //
        #define XLN_LOG //

    #endif
#endif

#ifndef XLN_SHIPPING
    #define __XLN_LOGW__ XCr::LogWarning
    #define XLN_LOGW XCr::Log::SetLogInfo(__FILE__, __FUNCTION__, __LINE__)?NULL:__XLN_LOGW__
#else
    #if defined (_MSC_VER) && (_MSC_VER >= 1300)
        #define XLN_LOGW __noop
    #else
        #define XLN_LOGW ((void)0)
    #endif
#endif


#define __XLN_LOGE__ XCr::LogError
#define XLN_LOGE XCr::Log::SetLogInfo(__FILE__, __FUNCTION__, __LINE__)?NULL:__XLN_LOGE__
