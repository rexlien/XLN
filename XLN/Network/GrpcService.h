#pragma once

#include <XLN/Core/Config/Config.h>
#include <nlohmann/json.hpp>
#include <grpcpp/grpcpp.h>
#include <folly/concurrency/ConcurrentHashMap.h>
#include <folly/futures/Future.h>
#include <XLN/Core/Foundation/ThreadPool.h>
#include <google/protobuf/message.h>
#include <XLN/Core/Foundation/Service.h>

namespace XLN {

    class XLN_CLASS_DECL GrpcService : public XCr::Service {

        XLN_DECLARE_SERVICE_CLASS(GrpcService);
        XLN_DECLARE_CREATOR(GrpcService);
    public:

        class GrpcCallReturn
        {
        public:
            grpc::Status status;
            std::shared_ptr<google::protobuf::Message> message;
        };

        class GrpcCallContextBase
        {
        public:
            virtual void promise() = 0;

            std::shared_ptr<GrpcCallReturn> ret;
        };

        template<class T>
        class GrpcCallContext : public GrpcCallContextBase
        {
        public:
            GrpcCallContext() {
                ret = std::make_shared<GrpcCallReturn>();
                ret->message = std::make_shared<T>();
            }

            virtual void promise() {
                replyPromise.setValue(ret);
            }
           
            folly::Promise<std::shared_ptr<GrpcCallReturn>> replyPromise;
            grpc::ClientContext context;

            std::unique_ptr<grpc::ClientAsyncResponseReader<T>> response_reader;

        };

        GrpcService();


        virtual bool OnInit() override;
        virtual bool OnDestroy() override;
   
        void CreateChannel(nlohmann::json config);
        std::shared_ptr<grpc::Channel> CreateChannel(const std::string& name, const std::string& address);
        std::shared_ptr<grpc::Channel> GetChannel(const std::string& name);

        template<class T> std::unique_ptr<typename T::Stub> CreateStub(const std::string& name)
        {
            return T::NewStub(GetChannel(name));
        }

        template<class T, class R> folly::Future<std::shared_ptr<GrpcCallReturn>> CallAsync(R& req,
                std::function<std::unique_ptr<grpc::ClientAsyncResponseReader<T>>(grpc::ClientContext*, const R&, grpc::CompletionQueue*)> requestFunction) {

            GrpcCallContext<T>* context = new GrpcCallContext<T>();
            //auto context = std::make_shared<GrpcCallContext<T>>();
            context->response_reader = requestFunction(&(context->context), req, &m_ClientCq);
            context->response_reader->StartCall();
            context->response_reader->Finish((T*)(context->ret->message.get()), &(context->ret->status), (void*)context);

            return context->replyPromise.getFuture();
            //return context;
        }

    protected:
        virtual void RunClient(XCr::ThreadPool::ThreadPtr thread);

        XCr::ThreadPool m_ClientThreadPool;
        grpc::CompletionQueue m_ClientCq;

    private:

        folly::ConcurrentHashMap<std::string, std::shared_ptr<grpc::Channel>> m_ChannelPool;


    };

}