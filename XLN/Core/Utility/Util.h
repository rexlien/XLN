#pragma once


#include <XLN/Core/Config/Config.h>

namespace XCr
{

    class XLN_CLASS_DECL TimeUtil
    {
        
    public:
        static std::string CurTimeToString();
        static std::string TimeToString( std::chrono::time_point<std::chrono::system_clock> time);

        static XCr::UInt32 GetTickCount();
        static std::chrono::steady_clock::time_point GetCurSteadyTimePoint();
        
    };

    class XLN_CLASS_DECL ThreadUtil
    {
    public:
        static XCr::UInt64 GetCurThreadID();
    };
    
}