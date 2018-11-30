#include "TestServerSystemFactoryService.h"
#include <XLN/Server/ServerApplication.h>
#include <XLN/Server/Thrift/ThriftApplication.h>
#include <XLN/Server/Thrift/ThriftMuxService.h>
#include <thrift/lib/cpp2/server/BaseThriftServer.h>

#include "gen-cpp2/EntityService.h"

XLN_IMPL_SERVICE_CLASS(TestServerSystemFactoryService, XGf::SystemFactoryService);
XLN_IMPLEMENT_CREATOR(TestServerSystemFactoryService);

using namespace apache::thrift;

class EntityService : public XLN::Game::Thrift::cpp2::EntityServiceSvIf
{
    virtual void sendMessage(std::unique_ptr< XLN::Game::Thrift::cpp2::EntityMessage> msg)
    {
        std::cout << "Entity Message received" << std::endl;
    }
};


TestServerSystemFactoryService::TestServerSystemFactoryService() {

}

TestServerSystemFactoryService::~TestServerSystemFactoryService() {

}



XGf::Application *TestServerSystemFactoryService::CreateApplication() {

    std::cout << "Creating Server application" << std::endl;
    auto entityService = std::make_shared<EntityService>();

    auto asyncProcessorFactory = std::make_shared<ThriftServerAsyncProcessorFactory<EntityService>>(entityService);
    return XLN_OBJ_NEW XLN::ThriftApplication(asyncProcessorFactory);

}

