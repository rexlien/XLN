#include <XLN/Network/GrpcServer.h>

#include "../proto-gen/Test/TestGrpc.grpc.pb.h"
#include "grpcpp/grpcpp.h"
#include "gtest/gtest.h"


using namespace helloworld;

class HelloService : public helloworld::Greeter::Service {

    grpc::Status SayHello(grpc::ServerContext* context, const HelloRequest* request,
        HelloReply* reply) override {


        return grpc::Status::OK;
    }
};


TEST(TestGRPC, SimpleRPC)
{
    std::string serverAddress("0.0.0.0:50051");
    XLN::GrpcServer testServer(std::make_unique<HelloService>());

    testServer.StartServer("test", serverAddress);

    auto channel = grpc::CreateChannel(
        "localhost:50051", grpc::InsecureChannelCredentials());
    auto stub = Greeter::NewStub(channel);
    HelloRequest request;
    request.set_name("test");

    HelloReply reply;
    grpc::ClientContext context;
    grpc::Status res = stub->SayHello(&context, request, &reply);
    GTEST_ASSERT_EQ(res.ok(), true);
    testServer.StopServer();
}