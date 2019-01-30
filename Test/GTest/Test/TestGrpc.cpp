#include <XLN/Network/GrpcServer.h>
#include <XLN/Network/GrpcService.h>
#include <folly/executors/GlobalExecutor.h>
#include <XLN/Core/Foundation/ServiceMgr.h>

#include "../proto-gen/Test/TestGrpc.grpc.pb.h"
#include "grpcpp/grpcpp.h"
#include "gtest/gtest.h"


using namespace helloworld;

class HelloService : public helloworld::Greeter::Service {

    grpc::Status SayHello(grpc::ServerContext* context, const HelloRequest* request,
        HelloReply* reply) override {

        reply->set_message("processed");
        return grpc::Status::OK;
    }
};


TEST(TestGRPC, SimpleRPC)
{
    std::string serverAddress("0.0.0.0:50051");
    XLN::GrpcServer testServer(std::make_unique<HelloService>());

    testServer.StartServer("test", serverAddress);


    XLN::GrpcService& client = *(XCr::ServiceMgr::GetActiveServiceMgr()->GetService<XLN::GrpcService>());
    auto channel = client.CreateChannel("test", "localhost:50051");
    auto stub = client.CreateStub<Greeter>("test");

    HelloRequest request;
    request.set_name("test");
     
    auto func = std::bind(&(helloworld::Greeter::Stub::PrepareAsyncSayHello), stub.get(), std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    auto future = client.CallAsync<HelloReply, HelloRequest>(request, func);
    auto reply = future.via(folly::getCPUExecutor().get()).then([](std::shared_ptr<XLN::GrpcService::GrpcCallReturn> ret) { 
            return ret;
    }).get();

    auto future2 = client.CallAsync<HelloReply, HelloRequest>(request, func).via(folly::getCPUExecutor().get()).
        then([](std::shared_ptr<XLN::GrpcService::GrpcCallReturn> ret) {
        return ret;
    });
    
  
    GTEST_ASSERT_EQ(reply->status.ok(), true);
    folly::collectAll(future2).wait();
    testServer.StopServer();
}