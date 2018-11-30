#pragma once

#include <XLN/Core/Config/Config.h>
#include <XLN/Core/Framework/SystemFactoryService.h>

class TestServerClientSystemFactoryService : public XGf::SystemFactoryService
{
    XLN_DECLARE_SERVICE_CLASS(TestServerClientSystemFactoryService)
    XLN_DECLARE_CREATOR(TestServerClientSystemFactoryService);

public:

	TestServerClientSystemFactoryService();
    virtual ~TestServerClientSystemFactoryService();

    virtual XGf::Application *CreateApplication() override;


};
