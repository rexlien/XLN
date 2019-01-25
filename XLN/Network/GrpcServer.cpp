#include "GrpcServer.h"
#include <XLN/Core/Foundation/Log.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/impl/codegen/service_type.h>


using namespace XLN;

GrpcServer::GrpcServer(std::unique_ptr<grpc::Service> service)
{
    m_Service = std::move(service);
}

void GrpcServer::StartServer(const std::string & serverName, nlohmann::json config)
{

    

}

void GrpcServer::StartServer(const std::string & serverName, const std::string& address)
{
    m_Address = address;
    m_ThreadPtr = m_ThreadPool.AddThread(std::bind(&GrpcServer::RunServer, this, std::placeholders::_1));
    m_ThreadPtr->WaitRun();

}

void GrpcServer::StopServer()
{
    if (!m_ServerStarted)
    {
        XLN_LOGD("Server not started yet");
        return;
    }
    m_Server->Shutdown();
    m_ThreadPtr->WaitExit();
    m_ThreadPool.StopAllThread();
}

void GrpcServer::RunServer(XCr::ThreadPool::ThreadPtr thread)
{
    
    grpc::ServerBuilder builder;
    grpc::Service* service = m_Service.get();
    builder.AddListeningPort(this->m_Address, grpc::InsecureServerCredentials());
    // Register "service" as the instance through which we'll communicate with
    // clients. In this case it corresponds to an *synchronous* service.
    builder.RegisterService(service);
    m_Server = std::move(builder.BuildAndStart());
    m_ServerStarted = true;
    
    thread->AfterRun();
    m_Server->Wait();
    m_ThreadPool.AddStoppingThread(thread);
    thread->BeforeExit();


}


