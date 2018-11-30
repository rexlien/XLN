#import <Foundation/Foundation.h>

#include "PlatformServiceiOS.h"
#include <XLN/Core/Foundation/Log.h>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string/replace.hpp>

#include <sys/sysctl.h>
#include <sys/utsname.h>

#import <UIKit/UIKit.h>
#import <CoreTelephony/CTTelephonyNetworkInfo.h>
#import <CoreTelephony/CTCarrier.h>

using namespace XCr;

XLN_IMPL_SERVICE_CLASS(PlatformServiceiOS, XCr::PlatformService);


PlatformServiceiOS::PlatformServiceiOS()
{
    
}

PlatformServiceiOS::~PlatformServiceiOS()
{
    
}

int PlatformServiceiOS::GetCPUCount() {
    size_t len;
    int ncpu;
    
    len = sizeof(ncpu);
    sysctlbyname ("hw.ncpu",&ncpu,&len,NULL,0);
    
    return ncpu;
}

XCr::String PlatformServiceiOS::GetOSName() {
    
    NSString *strSysName = [[UIDevice currentDevice] systemName];
    NSString *strSysVersion = [[UIDevice currentDevice] systemVersion];
    return std::string([strSysName UTF8String]) + std::string([strSysVersion UTF8String]);
    
}

XCr::String PlatformServiceiOS::GetDeviceModel() {
    
    struct utsname systemInfo;
    uname(&systemInfo);
    return systemInfo.machine;
}

XCr::String PlatformServiceiOS::GetCarrierName() {
    
    CTTelephonyNetworkInfo *netinfo = [[CTTelephonyNetworkInfo alloc] init];
    CTCarrier *carrier = [netinfo subscriberCellularProvider];
    std::string info = [[carrier carrierName] UTF8String];
    
    return info;
}

XCr::String PlatformServiceiOS::GetNetworkType() {
    
    CTTelephonyNetworkInfo *telephonyInfo = [[CTTelephonyNetworkInfo alloc] init];
    if(telephonyInfo.currentRadioAccessTechnology == NULL)
    {
        return "Unknown";
    }
    
    std::string tech = std::string("-") + [telephonyInfo.currentRadioAccessTechnology UTF8String];
    if ([telephonyInfo.currentRadioAccessTechnology isEqualToString:CTRadioAccessTechnologyGPRS]) {
        return "2G" + tech;
    } else if ([telephonyInfo.currentRadioAccessTechnology  isEqualToString:CTRadioAccessTechnologyEdge]) {
        return "2G" + tech;
    } else if ([telephonyInfo.currentRadioAccessTechnology  isEqualToString:CTRadioAccessTechnologyWCDMA]) {
        return "3G" + tech;
    } else if ([telephonyInfo.currentRadioAccessTechnology  isEqualToString:CTRadioAccessTechnologyHSDPA]) {
        return "3G" + tech;
    } else if ([telephonyInfo.currentRadioAccessTechnology  isEqualToString:CTRadioAccessTechnologyHSUPA]) {
        return "3G" + tech;
    } else if ([telephonyInfo.currentRadioAccessTechnology  isEqualToString:CTRadioAccessTechnologyCDMA1x]) {
        return "3G" + tech;
    } else if ([telephonyInfo.currentRadioAccessTechnology  isEqualToString:CTRadioAccessTechnologyCDMAEVDORev0]) {
        return "3G" + tech;
    } else if ([telephonyInfo.currentRadioAccessTechnology  isEqualToString:CTRadioAccessTechnologyCDMAEVDORevA]) {
        return "3G" + tech;
    } else if ([telephonyInfo.currentRadioAccessTechnology  isEqualToString:CTRadioAccessTechnologyCDMAEVDORevB]) {
        return "3G" + tech;
    } else if ([telephonyInfo.currentRadioAccessTechnology  isEqualToString:CTRadioAccessTechnologyeHRPD]) {
        return "3G" + tech;
    } else if ([telephonyInfo.currentRadioAccessTechnology  isEqualToString:CTRadioAccessTechnologyLTE]) {
        return "4G" + tech;
    }
}

template<>
void PlatformService::GetSignatureValueOne<XCr::String>( XCr::String& signature, folly::dynamic& object, const::XCr::String& arg)
{

    object.push_back(arg);
    
}


template<>
void PlatformService::GetSignatureValueReturn<void>(XCr::String& signature, folly::dynamic& ret)
{
    ret = nullptr;
}

template<>
void PlatformService::GetSignatureValueReturn<XCr::String>(XCr::String& signature, folly::dynamic& ret)
{
    ret = "";
}


XCr::String PlatformServiceiOS::GetWritableRootPath() const
{
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentsDirectory = [paths objectAtIndex:0];
    XCr::String strRet = [documentsDirectory UTF8String];
    strRet.append("/");
    return strRet;

}


void PlatformServiceiOS::CallPlatformMethodImpl(const XCr::String& className, const XCr::String& method, const XCr::String& signature, folly::dynamic& ret, const folly::dynamic& object)
{
    XCr::String objClassName = className;
    boost::replace_all(objClassName, "::", "_");
    
    NSString* cClazz = [NSString stringWithCString:objClassName.c_str() encoding:[NSString defaultCStringEncoding]];
    NSString* cMethod = [NSString stringWithCString:method.c_str() encoding:[NSString defaultCStringEncoding]];
    
    Class clazz = NSClassFromString(cClazz);
    SEL selector = NSSelectorFromString(cMethod);
    
    if(clazz)
    {
        NSMethodSignature *sig = [clazz methodSignatureForSelector:selector];
        if(sig)
        {   NSInvocation *invocation = [NSInvocation invocationWithMethodSignature:sig];
            [invocation setSelector:selector];
            [invocation setTarget:clazz];
            
            int i = 2;
            for (auto& val : object)
            {
                if(val.isString())
                {
                    NSString* arg = [NSString stringWithCString:val.asString().c_str() encoding:[NSString defaultCStringEncoding]];
                    [invocation setArgument:&arg atIndex:i];
                }
                i++;
            }
            [invocation invoke];
            
            if(!ret.isNull())
            {
                if(ret.isString())
                {
                    NSString *resultSet = nil;
                    [invocation getReturnValue:&resultSet];
                    if(resultSet)
                    {
                        ret = [resultSet UTF8String];
                    }
                }
            }
        }
        
    }
}


