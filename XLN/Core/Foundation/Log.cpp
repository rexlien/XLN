#include "Log.h"


#include <XLN/Core/Foundation/Service/LogService.h>
#include <XLN/Core/Foundation/FoundationSDM.h>
#include <XLN/Core/Foundation/ServiceMgr.h>
#include <XLN/Core/Foundation/Service/LogService.h>
#include <XLN/Core/Utility/Util.h>
#include <XLN/Core/Platform/PlatformService.h>


#include <stdarg.h>
#include "spdlog/spdlog.h"


using namespace XCr;

static folly::ThreadLocal<char*> s_CurFuncName;
static folly::ThreadLocal<char*> s_CurFileName;
static folly::ThreadLocal<int> s_CurLineNum;


static std::mutex s_CriticalSection;

static LogLevel s_CurLogLevel = LL_LOG_INFO;
static XCr::LogService* s_LogService = 0;


static std::string GetTimeLabel()
{
    return XCr::TimeUtil::CurTimeToString();
}

static void PrependLogInfo(char* logBuf, const char* fileName, const char* funcName, int lineNum, const char* prefix)
{
    char prefixBuf[512];
    std::string timeLabel = XCr::TimeUtil::CurTimeToString();
    snprintf(prefixBuf, 512, "[%s][tag][TID%" PRIu64 "][%s][%s:%s:%d] %s", prefix, XCr::ThreadUtil::GetCurThreadID(), timeLabel.c_str(), fileName, funcName, lineNum, logBuf);
    memcpy(logBuf, prefixBuf, 512);

    //int prefixLen = (int)strlen(prefixBuf);
    //int msgLen = (int)strlen(logBuf);
    //if(prefixLen + msgLen >= 512)
    //    return;

    //memmove((void*)(logBuf + prefixLen), (void*)logBuf, msgLen);
    //memmove((void*)logBuf, (void*)prefixBuf, prefixLen);
    //logBuf[prefixLen+msgLen] = '\0';

}

static void PrependLogInfo(char* logBuf, const char* fileName, const char* funcName, int lineNum, const char* prefix, const char* tag)
{
    char prefixBuf[512];
    std::string timeLabel = XCr::TimeUtil::CurTimeToString();
    snprintf(prefixBuf, 512, "[%s][%s][TID%" PRIu64 "][%s][%s:%s:%d] %s", prefix, tag, XCr::ThreadUtil::GetCurThreadID(), timeLabel.c_str(), fileName, funcName, lineNum);
    memcpy(logBuf, prefixBuf, 512);
    //int prefixLen = (int)strlen(prefixBuf);
    //int msgLen = (int)strlen(logBuf);
    //if(prefixLen + msgLen >= 512)
    //    return;

    //memmove((void*)(logBuf + prefixLen), (void*)logBuf, msgLen);
    //memmove((void*)logBuf, (void*)prefixBuf, prefixLen);
    //logBuf[prefixLen+msgLen] = '\0';

}

static void DoLog(LogLevel level, const std::string& log)
{
    if(!s_LogService)
    {
        s_CriticalSection.lock();
        if(!s_LogService)
        {
            XCr::ServiceMgr* serviceMgr = XCr::FoundationSDM::GetCoreServiceMgr();
            if(serviceMgr)
            {   s_LogService = XCr::FoundationSDM::GetCoreServiceMgr()->GetService<LogService>();
            }
            
        }
        s_CriticalSection.unlock();
    }
    
    if(s_LogService)
        s_LogService->Log(level, log);
    
}




Log::Log(const char* pcFormat, ...)
{   

}


Log::~Log(void)
{
}

void Log::SetLogLevel( LogLevel level )
{
    std::lock_guard<std::mutex> guard(s_CriticalSection);
    s_CurLogLevel = level;
    
}



XCr::LogError::LogError( const char* pcFormat, ... )
{
    if(s_CurLogLevel <= XCr::LogLevel::LL_LOG_ERROR)
    {
        va_list kArgList;
        va_start(kArgList,pcFormat);
        
        char buffer[512];
        vsnprintf (buffer, 512, pcFormat, kArgList);
        
        va_end(kArgList);
        PrependLogInfo(buffer, *s_CurFileName.get(), (s_CurFuncName.get() == nullptr)?"UnKnown":*(s_CurFuncName.get()), *s_CurLineNum, "Error");
        
        DoLog(LL_LOG_ERROR, buffer);
    }

    
}

XCr::LogError::LogError( const char* tag, const char* pcFormat, ... )
{
    if(s_CurLogLevel <= XCr::LogLevel::LL_LOG_ERROR)
    {
        va_list kArgList;
        va_start(kArgList,pcFormat);
        
        char buffer[512];
        vsnprintf (buffer, 512, pcFormat, kArgList);
        
        va_end(kArgList);
        PrependLogInfo(buffer, *s_CurFileName.get(), (s_CurFuncName.get() == nullptr)?"UnKnown":*(s_CurFuncName.get()), *s_CurLineNum, tag);
        
        DoLog(LL_LOG_ERROR, buffer);
    }

}

XCr::LogWarning::LogWarning( const char* pcFormat, ... )
{
    if(s_CurLogLevel <= XCr::LogLevel::LL_LOG_WARNING)
    {
        va_list kArgList;
        va_start(kArgList,pcFormat);
        
        char buffer[512];
        vsnprintf (buffer, 512, pcFormat, kArgList);
        
        va_end(kArgList);
        PrependLogInfo(buffer, *s_CurFileName.get(), (s_CurFuncName.get() == nullptr)?"UnKnown":*(s_CurFuncName.get()), *s_CurLineNum, "Warning");
        
        DoLog(LL_LOG_WARNING, buffer);
    }

}

XCr::LogWarning::LogWarning( const char* tag, const char* pcFormat, ... )
{
    if(s_CurLogLevel <= XCr::LogLevel::LL_LOG_WARNING)
    {
        va_list kArgList;
        va_start(kArgList,pcFormat);
        
        char buffer[512];
        vsnprintf (buffer, 512, pcFormat, kArgList);
        
        va_end(kArgList);
        PrependLogInfo(buffer, *s_CurFileName.get(), (s_CurFuncName.get() == nullptr)?"UnKnown":*(s_CurFuncName.get()), *s_CurLineNum, tag);
        
        DoLog(LL_LOG_WARNING, buffer);
    }
}


XCr::LogDebug::LogDebug( const char* pcFormat, ... )
{
    if(s_CurLogLevel <= XCr::LogLevel::LL_LOG_DEBUG)
    {
        va_list kArgList;
        va_start(kArgList,pcFormat);
    
        char buffer[512];
        vsnprintf (buffer, 512, pcFormat, kArgList);
    
        va_end(kArgList);
        PrependLogInfo(buffer, *s_CurFileName.get(), (s_CurFuncName.get() == nullptr)?"UnKnown":*(s_CurFuncName.get()), *s_CurLineNum, "Debug");

        DoLog(LL_LOG_DEBUG, buffer);
    }
    
}

XCr::LogDebug::LogDebug( const char* tag, const char* pcFormat, ... )
{
    if(s_CurLogLevel <= XCr::LogLevel::LL_LOG_DEBUG)
    {
        va_list kArgList;
        va_start(kArgList,pcFormat);
        
        char buffer[512];
        vsnprintf (buffer, 512, pcFormat, kArgList);
        
        va_end(kArgList);
        PrependLogInfo(buffer, *s_CurFileName.get(), (s_CurFuncName.get() == nullptr)?"UnKnown":*(s_CurFuncName.get()), *s_CurLineNum, tag);
        
        DoLog(LL_LOG_DEBUG, buffer);
    }

}

bool XCr::Log::SetLogInfo( const char* name, const char* funcName, int lineNum )
{
    *s_CurLineNum = lineNum;
    *s_CurFuncName =  const_cast<char*>(funcName);//.reset(const_cast<char*>(funcName));
    *s_CurFileName =  const_cast<char*>(name);
    
    return false;
}

XCr::LogInfo::LogInfo( const char* pcFormat, ... )
{
    if(s_CurLogLevel <= XCr::LogLevel::LL_LOG_INFO)
    {
        va_list kArgList;
        va_start(kArgList,pcFormat);
        
        char buffer[512];
        vsnprintf (buffer, 512, pcFormat, kArgList);
        
        va_end(kArgList);
        PrependLogInfo(buffer, *s_CurFileName.get(), (s_CurFuncName.get() == nullptr)?"UnKnown":*(s_CurFuncName.get()), *s_CurLineNum, "Info");
        
        DoLog(LL_LOG_INFO, buffer);
    }

}

XCr::LogInfo::LogInfo( const char* tag, const char* pcFormat, ... )
{
    if(s_CurLogLevel <= XCr::LogLevel::LL_LOG_INFO)
    {
        va_list kArgList;
        va_start(kArgList,pcFormat);
        
        char buffer[512];
        vsnprintf (buffer, 512, pcFormat, kArgList);
        
        va_end(kArgList);
        PrependLogInfo(buffer, *s_CurFileName.get(), (s_CurFuncName.get() == nullptr)?"UnKnown":*(s_CurFuncName.get()), *s_CurLineNum, tag);
        
        DoLog(LL_LOG_INFO, buffer);
    }
}