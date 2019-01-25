#include "System.h"

#include <XLN/Core/Foundation/ServiceMgr.h>
#include <XLN/Core/Foundation/FoundationSDM.h>
#include <XLN/Core/Framework/ExecutorService.h>
#include <XLN/Core/Framework/SystemFactoryService.h>
#include <XLN/Core/Framework/Application.h>
#include <XLN/Core/Framework/AssetService.h>
#include <XLN/Core/Framework/ConfigService.h>
#include <XLN/Core/Foundation/Service/LogService.h>

#include <folly/Singleton.h>

#if XLN_PLATFORM_APPLE
#include <XLN/Core/Platform/ios/PlatformServiceiOS.h>
#include <XLN/Core/Platform/ios/LogServiceiOS.h>
#elif (XLN_PLATFORM == XLN_PLATFORM_ANDROID)
#include <XLN/Core/Platform/android/src/main/cpp/PlatformServiceAndroid.h>
#include <XLN/Core/Platform/android/src/main/cpp/LogServiceAndroid.h>
#include <XLN/Core/Platform/android/src/main/cpp/Jni.h>
#include <android/log.h>
#elif (XLN_PLATFORM == XLN_PLATFORM_WIN32)
#include <XLN/Core/Platform/win/PlatformServiceWin.h>
#elif (XLN_PLATFORM == XLN_PLATFORM_LINUX)
#include <XLN/Core/Platform/linux/PlatformServiceLinux.h>
#endif

using namespace XGf;
using json = nlohmann::json;

static XGf::Application* s_Applicaiton = 0;
namespace
{
    void InjectService()
    {
        XGf::ConfigService* configService = XCr::ServiceMgr::GetActiveServiceMgr()->GetService<XGf::ConfigService>();
        if (configService->GetAppConfig() != nullptr)
            return;

        auto services = configService->GetAppConfig()["XLNAppConfig"]["Services"];
        if (services != nullptr)
        {
            for (json::iterator it = services.begin(); it != services.end(); ++it) {
               
                std::string classname = (*it)["Class"];
                XCr::ServiceMgr::GetActiveServiceMgr()->RegisterService(classname);
            }
        }
    }
}

void Init()
{
#if XLN_PLATFORM_APPLE
    XCr::ServiceMgr::GetActiveServiceMgr()->RegisterService(XLN_OBJ_NEW XCr::PlatformServiceiOS());
    XCr::ServiceMgr::GetActiveServiceMgr()->RegisterService(XLN_OBJ_NEW XCr::LogServiceiOS());
#elif (XLN_PLATFORM == XLN_PLATFORM_ANDROID)
    XCr::ServiceMgr::GetActiveServiceMgr()->RegisterService(XLN_OBJ_NEW XCr::PlatformServiceAndroid());
    XCr::ServiceMgr::GetActiveServiceMgr()->RegisterService(XLN_OBJ_NEW XCr::LogServiceAndroid());
#elif (XLN_PLATFORM == XLN_PLATFORM_WIN32)
    XCr::ServiceMgr::GetActiveServiceMgr()->RegisterService(XLN_OBJ_NEW XCr::PlatformServiceWin());
    XCr::ServiceMgr::GetActiveServiceMgr()->RegisterService(XLN_OBJ_NEW XCr::LogService());
#elif (XLN_PLATFORM == XLN_PLATFORM_LINUX)
    XCr::ServiceMgr::GetActiveServiceMgr()->RegisterService(XLN_OBJ_NEW XCr::PlatformServiceLinux());
    XCr::ServiceMgr::GetActiveServiceMgr()->RegisterService(XLN_OBJ_NEW XCr::LogService());
#endif
    
    XGf::AssetService* assetService = XLN_OBJ_NEW XGf::AssetService();
    assetService->SetAssetPath("asset");
    XCr::ServiceMgr::GetActiveServiceMgr()->RegisterService(assetService);

    XCr::ServiceMgr::GetActiveServiceMgr()->RegisterService(XLN_OBJ_NEW XGf::ConfigService());

    XGf::SystemFactoryService* factoryService = XCr::ServiceMgr::GetActiveServiceMgr()->GetService<XGf::SystemFactoryService>();
    if(factoryService)
    {
        s_Applicaiton = factoryService->CreateApplication();
    }
    else
    {
        XGf::SystemFactoryService* defaultFactory = XLN_OBJ_NEW XGf::SystemFactoryService();
        XCr::ServiceMgr::GetActiveServiceMgr()->RegisterService(defaultFactory);
        s_Applicaiton = defaultFactory->CreateApplication();
        //XLN_OBJ_NEW XGf::Application();

    }
    s_Applicaiton->InitService();

    XCr::ServiceMgr::GetActiveServiceMgr()->Init();
    s_Applicaiton->PostInitServices();
    InjectService();
    s_Applicaiton->Init();

}



bool System::Init(const XCr::String& factoryServiceName)
{
    
    XCr::FoundationSDM::Init();
    
    XCr::ServiceMgr::AddServiceMgr("CoreService", XCr::FoundationSDM::GetCoreServiceMgr());
    XCr::ServiceMgr::SetActiveServiceMgr("CoreService");

    XCr::ServiceMgr::GetActiveServiceMgr()->RegisterService(factoryServiceName);

    ::Init();

    XLN_LOGW("System Inited..");
    return true;
}

bool System::Init(XGf::SystemFactoryService* factoryService)
{
    XCr::FoundationSDM::Init();

    XCr::ServiceMgr::AddServiceMgr("CoreService", XCr::FoundationSDM::GetCoreServiceMgr());
    XCr::ServiceMgr::SetActiveServiceMgr("CoreService");

    XCr::ServiceMgr::GetActiveServiceMgr()->RegisterService(factoryService);
    ::Init();

    XLN_LOGW("System Inited..");
    return true;

}



void System::Shutdown()
{
    XGf::SystemFactoryService* factoryService = XCr::ServiceMgr::GetActiveServiceMgr()->GetService<XGf::SystemFactoryService>();
    factoryService->DestroyApplication(s_Applicaiton);
    XCr::ServiceMgr::GetActiveServiceMgr()->ShutDown();
    XCr::FoundationSDM::ShutDown();
	folly::SingletonVault::singleton()->destroyInstances();
}

void System::Main(int argc, char** argv)
{
    s_Applicaiton->Main(argc, argv);
}

#if (XLN_PLATFORM == XLN_PLATFORM_ANDROID)

extern "C"
{

JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved)
{
    //__android_log_print(ANDROID_LOG_FATAL, "app", "Jni_OnLoad");
    XCr::JniHelper::SetJavaVM(vm);
    XCr::JniHelper::SetClassLoader();



    return JNI_VERSION_1_4;
}

JNIEXPORT void Java_xln_core_Jni_AppMain(JNIEnv *env, jobject /* this */, jobject activity, jobject applicaiton, jstring systemFactoryServieName)
{
    //XGf::Application* cppApp = CreateApplication();
    //cppApp->OnInit();
    XCr::String cFatoryServeName = XCr::JniHelper::JavaString2String(systemFactoryServieName);
    System::Init(cFatoryServeName);
    int argc = 1;
    char* argv = const_cast<char*>("AndroidApp");
    System::Main(argc, &argv);

}


}
#endif
