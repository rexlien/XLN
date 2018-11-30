//
// Created by Rex Lien on 3/23/17.
//

#include "SystemFactoryService.h"
#include <XLN/Core/Framework/Application.h>

using namespace XGf;

XLN_IMPL_SERVICE_CLASS(SystemFactoryService, Service);

SystemFactoryService::SystemFactoryService()
{

}

SystemFactoryService::~SystemFactoryService() {

}


void SystemFactoryService::DestroyApplication(XGf::Application *application)
{
    XLN_OBJ_DELETE application;
}

XGf::Application *SystemFactoryService::CreateApplication() {
    return XLN_OBJ_NEW Application();
}

