//
// Created by Rex Lien on 3/23/17.
//

#pragma once

#include <XLN/Core/Config/Config.h>
#include <XLN/Core/Foundation/Service.h>
#include <folly/Function.h>

namespace XGf
{

class Application;

class XLN_CLASS_DECL SystemFactoryService : public XCr::Service
{
XLN_DECLARE_SERVICE_CLASS(SystemFactoryService);

public:
    SystemFactoryService();
    virtual ~SystemFactoryService();

    void RegisterServiceCreator(XCr::Service::ServiceID, folly::Function<void>);
    virtual XGf::Application* CreateApplication();
    virtual void DestroyApplication(XGf::Application* application);

private:
    //static std::map<XCr::Service::ServiceID, folly::Function<void>> m_ServiceCreators;
    //protected XGf::Application* m_Application;

};

}

