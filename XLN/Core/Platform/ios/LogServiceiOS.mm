#include "LogServiceiOS.h"
#include <XLN/Core/Foundation/FoundationSDM.h>
#include <XLN/Core/Foundation/ServiceMgr.h>
#include <XLN/Core/Platform/PlatformService.h>

#import <Foundation/Foundation.h>

using namespace XCr;

XLN_IMPL_SERVICE_CLASS(LogServiceiOS, XCr::LogService);

XCr::LogServiceiOS::LogServiceiOS()
{

}

XCr::LogServiceiOS::~LogServiceiOS()
{

}

bool LogServiceiOS::Log(XCr::LogLevel level, const XCr::String& msg)
{
    LogService::Log(level, msg);
    
    NSLog(@"%s", msg.c_str());

    return true;
    
}