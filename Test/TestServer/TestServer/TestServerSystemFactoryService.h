#pragma once

#include <XLN/Core/Config/Config.h>
#include <XLN/Core/Framework/SystemFactoryService.h>

class TestServerSystemFactoryService : public XGf::SystemFactoryService
{
    XLN_DECLARE_SERVICE_CLASS(TestServerSystemFactoryService)
    XLN_DECLARE_CREATOR(TestServerSystemFactoryService);

public:

    TestServerSystemFactoryService();
    virtual ~TestServerSystemFactoryService();

    virtual XGf::Application *CreateApplication() override;


};
