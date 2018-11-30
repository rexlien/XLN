//
// Created by Rex Lien on 3/16/17.
//

#include "Application.h"
#include <glog/logging.h>
#include <XLN/Core/Foundation/ServiceMgr.h>
#include <XLN/Core/Platform/PlatformService.h>
#include <folly/init/Init.h>

using namespace XGf;


Application::Application() {

}


Application::~Application() {

}

void Application::Init() {
    
    XCr::PlatformService* platformService = XCr::ServiceMgr::GetActiveServiceMgr()->GetService<XCr::PlatformService>();
    if (platformService)
    {
        std::string glogPath = platformService->GetWritableRootPath() + "Glog//";
        if(platformService->MakeDirectory(glogPath))
            FLAGS_log_dir = glogPath;
    }

    //FLAGS_logtostderr = true;
    FLAGS_v = 5;//for folly vlog

    //google::InitGoogleLogging("XLN");
    
    OnInit();
}


void Application::OnInit() {

}


void Application::PostInitServices()
{
    OnPostInitServices();
}

void Application::OnPostInitServices()
{

}


void Application::Main(int argc, char** argv) {

    //int argc = 1;
    //char* argv = const_cast<char*>("XLNGTest");
    //char** aargv = &argv;
    folly::init(&argc, &argv);
}


void Application::InitService()
{
    OnInitService();
}

void Application::OnInitService() {

}


