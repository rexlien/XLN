#pragma once

#include <XLN/Core/Config/Config.h>
#include <XLN/Core/Foundation/ThreadPool.h>
#include <nlohmann/json.hpp>
#include <grpcpp/grpcpp.h>
#include <folly/Baton.h>

namespace XLN
{

    class GrpcServer
    {

    public:
        GrpcServer(std::unique_ptr<grpc::Service> service);
        virtual void StartServer(const std::string& serverName, nlohmann::json config);
        virtual void StartServer(const std::string& serverName, const std::string& address);

        virtual void StopServer();

        //virtual void BindService()


    protected:
        virtual void RunServer(XCr::ThreadPool::ThreadPtr thread);
    private:
        XCr::ThreadPool m_ThreadPool;
        XCr::ThreadPool::ThreadPtr m_ThreadPtr;
        std::string m_Address;
        std::unique_ptr<grpc::Service> m_Service;
        std::unique_ptr<grpc::Server> m_Server;
        std::atomic<bool> m_ServerStarted{ false };
        folly::Baton<> m_ServerRunningBarrier;
    };

}