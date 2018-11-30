#include "TestServerClientSystemFactoryService.h"
#include <XLN/Server/ServerApplication.h>

XLN_IMPL_SERVICE_CLASS(TestServerClientSystemFactoryService, XGf::SystemFactoryService);
XLN_IMPLEMENT_CREATOR(TestServerClientSystemFactoryService);

TestServerClientSystemFactoryService::TestServerClientSystemFactoryService() {

}

TestServerClientSystemFactoryService::~TestServerClientSystemFactoryService() {

}


XGf::Application *TestServerClientSystemFactoryService::CreateApplication() {
    //XLN_LOGE("TestServer", "Creating Server application");
    std::cout << "Creating Server application" << std::endl;
    return XLN_OBJ_NEW XLN::ServerApplication();

}

