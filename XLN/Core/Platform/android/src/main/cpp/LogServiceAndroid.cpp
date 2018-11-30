//
// Created by Rex Lien on 2/16/17.
//

#include "LogServiceAndroid.h"
#include <android/log.h>

using namespace XCr;

XLN_IMPL_SERVICE_CLASS(LogServiceAndroid, XCr::LogService);

LogServiceAndroid::LogServiceAndroid() {

}

LogServiceAndroid::~LogServiceAndroid()
{

}

bool LogServiceAndroid::Log(XCr::LogLevel level, const XCr::String &msg) {

    switch(level)
    {
        case XCr::LogLevel::LL_LOG_DEBUG:
        {
            __android_log_print(ANDROID_LOG_DEBUG, "LGEngineLog", "%s", msg.c_str());
            break;
        }
        case XCr::LogLevel ::LL_LOG_INFO:
        {
            __android_log_print(ANDROID_LOG_INFO, "LGEngineLog", "%s", msg.c_str());
            break;
        }
        case XCr::LogLevel::LL_LOG_WARNING:
        {
            __android_log_print(ANDROID_LOG_WARN, "LGEngineLog", "%s", msg.c_str());
            break;
        }
        case XCr::LogLevel ::LL_LOG_ERROR:
        {
            __android_log_print(ANDROID_LOG_ERROR, "LGEngineLog", "%s", msg.c_str());
            break;
        }


    }

    return LogService::Log(level, msg);
}

