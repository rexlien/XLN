#pragma once

#include <XLN/Core/Config/Config.h>
#include <XLN/Core/Foundation/Service.h>
#include <XLN/Core/Foundation/Task/EventHandler.h>
#include <XLN/React/WebViewMessage.h>

namespace XGf {

class ReactService : public XCr::Service
{
    XLN_DECLARE_SERVICE_CLASS(ReactService)
    virtual bool OnInit() override;
    virtual void GetDependency(std::vector<ServiceID> &dependencies) override;;

public:
    virtual void OnHandleMessage(XCr::SharedPtr<XCr::IMessage> message);

private:
    XCr::SharedPtr<XCr::MessageHandlerHelper<ReactService, XGf::WebViewMessage>> m_MessageHandler;
};

}