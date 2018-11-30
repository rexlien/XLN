#include "LogService.h"
#include <XLN/Core/Foundation/FoundationSDM.h>
#include <XLN/Core/Foundation/ServiceMgr.h>
#include <XLN/Core/Utility/Util.h>
#include <XLN/Core/Platform/PlatformService.h>

#include "spdlog/spdlog.h"

using namespace XCr;

XLN_IMPL_SERVICE_CLASS(LogService, XCr::Service);

static std::shared_ptr<spdlog::logger> s_Logger;

XCr::LogService::LogService()
{
    m_PrintStdOut = false;
}

XCr::LogService::~LogService()
{

}

bool LogService::OnInit()
{
    if(!s_Logger)
    {
        XCr::ServiceMgr* serviceMgr = XCr::FoundationSDM::GetCoreServiceMgr();
        if(serviceMgr)
        {
            XCr::PlatformService* platfromService = serviceMgr->GetService<XCr::PlatformService>();
            if(platfromService)
            {   std::string fileName = std::string("XLN-") + XCr::TimeUtil::CurTimeToString() + std::string(".txt");
                try {
                    s_Logger = spdlog::basic_logger_mt("XLNLogger",
                                                       platfromService->GetXLNRoot() +
                                                       fileName);
                    s_Logger->set_level(spdlog::level::debug);
                    s_Logger->set_pattern("%v");
                    s_Logger->flush_on(spdlog::level::warn);
                }catch(spdlog::spdlog_ex& ex)
                {
                    XLN_LOGW("LogService", ex.what());
                }


            }
        }
    }

    return true;
}

bool XCr::LogService::OnDestroy()
{
    if (s_Logger)
    {
        s_Logger->flush();
    }
    return true;
}

void LogService::GetDependency(std::vector<ServiceID>& dependencies)
{
    dependencies.push_back(Service::GetServiceID<PlatformService>());
}


bool LogService::Log(XCr::LogLevel level, const XCr::String& msg)
{
    if (m_PrintStdOut)
    {
        std::cout << "Level:" << level << "--" << msg.c_str() << std::endl;
    }
    if(s_Logger)
    {
        switch(level)
        {   case XCr::LogLevel::LL_LOG_INFO:
            {   s_Logger->info("{}", msg.c_str());
                break;
            }
            case XCr::LogLevel::LL_LOG_DEBUG:
            {
                s_Logger->debug("{}", msg.c_str());
                break;
            }
            case XCr::LogLevel::LL_LOG_WARNING:
            {
                s_Logger->warn("{}", msg.c_str());
                break;
            }
            case XCr::LogLevel::LL_LOG_ERROR:
            {
                s_Logger->error("{}", msg.c_str());
                break;
            }
            default:
                break;
        }
        
        //s_Logger->flush();
        
    }
    return true;
}