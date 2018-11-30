#pragma once

#include <XLN/Core/Config/Config.h>
#include <XLN/Server/ServerApplication.h>
#include <thrift/lib/cpp/util/ScopedServerThread.h>
#include <thrift/lib/cpp2/async/AsyncProcessor.h>

namespace XLN
{
    class XLN_CLASS_DECL ThriftApplication : public XLN::ServerApplication
    {

    public:
        ThriftApplication(std::shared_ptr<apache::thrift::AsyncProcessorFactory> processorFactory);
        void EnableAcceptWebScoket(bool accept);
        
    protected:
        virtual void StartServer(int argc, char** argv) override;
        virtual void StopServer() override;

    private:
        apache::thrift::util::ScopedServerThread m_ThriftThread;
        std::shared_ptr<apache::thrift::AsyncProcessorFactory> m_ProcessorFactory;
        bool m_AcceptWebSocket;

    };


}