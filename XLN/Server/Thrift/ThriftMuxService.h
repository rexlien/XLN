#pragma once

#include <XLN/Core/Config/Config.h>
#include <thrift/lib/cpp2/async/AsyncProcessor.h>
#include <thrift/lib/cpp2/server/Cpp2ConnContext.h>
#include <thrift/lib/cpp2/async/ResponseChannel.h>
#include <thrift/lib/cpp/transport/THeader.h>
#include <thrift/lib/cpp2/protocol/BinaryProtocol.h>
#include <thrift/lib/cpp2/protocol/CompactProtocol.h>
#include <thrift/lib/cpp2/protocol/Frozen2Protocol.h>
#include <thrift/lib/cpp2/GeneratedCodeHelper.h>


namespace XLN
{

    class ThriftMuxProcessor : public apache::thrift::AsyncProcessor
    {
    public:
        ThriftMuxProcessor(const std::vector<std::shared_ptr<apache::thrift::ServerInterface>>& services);

        
        virtual void process(std::unique_ptr<apache::thrift::ResponseChannel::Request> req,
            std::unique_ptr<folly::IOBuf> buf,
            apache::thrift::protocol::PROTOCOL_TYPES protType,
            apache::thrift::Cpp2RequestContext* context,
            folly::EventBase* eb,
            apache::thrift::concurrency::ThreadManager* tm) override;

        virtual bool isOnewayMethod(const folly::IOBuf* buf, const apache::thrift::transport::THeader* header) override;



    protected:
        const std::vector<std::shared_ptr<apache::thrift::ServerInterface>>& m_Services;
        std::map<std::string, std::unique_ptr<apache::thrift::AsyncProcessor>> m_Processors;
        std::map<std::string, std::unique_ptr<apache::thrift::AsyncProcessor>> m_FunctionProcessorMap;
       
    };

    template<class T, class... Ts>
    class ThriftMuxProcessorTemplate : public ThriftMuxProcessorTemplate<Ts...>
    {
    public:
        ThriftMuxProcessorTemplate(const std::vector<std::shared_ptr<apache::thrift::ServerInterface>>& services)
            : ThriftMuxProcessorTemplate<Ts...>(services)
        {
          
            typename T::ProcessorType* processor = nullptr;
            for (auto& kv : ThriftMuxProcessor::m_Processors)
            {
                processor = dynamic_cast<typename T::ProcessorType*>(kv.second.get());
                if (processor)
                {
                    ThriftMuxProcessorTemplate<T, Ts...>::m_CurProcessor = processor;
                    break;
                }

            }
            
        }
        virtual void process(std::unique_ptr<apache::thrift::ResponseChannel::Request> req,
            std::unique_ptr<folly::IOBuf> buf,
            apache::thrift::protocol::PROTOCOL_TYPES protType,
            apache::thrift::Cpp2RequestContext* context,
            folly::EventBase* eb,
            apache::thrift::concurrency::ThreadManager* tm) override
        {   
            //find if it's able to process methodName using binary protocol map...
            if (T::ProcessorType::getBinaryProtocolProcessMap().find(context->getMethodName()) != T::ProcessorType::getBinaryProtocolProcessMap().end())
            {
                apache::thrift::detail::ap::process(ThriftMuxProcessorTemplate<T, Ts...>::m_CurProcessor, std::move(req), std::move(buf), protType, context, eb, tm);
            }
            else
            {
                ThriftMuxProcessorTemplate<Ts...>::process(std::move(req), std::move(buf), protType, context, eb, tm);
            }
        }
    private:
        typename T::ProcessorType * m_CurProcessor;

    };

    template<class T>
    class ThriftMuxProcessorTemplate<T> : public ThriftMuxProcessor
    {
    public:
        ThriftMuxProcessorTemplate(const std::vector<std::shared_ptr<apache::thrift::ServerInterface>>& services)
            :   ThriftMuxProcessor(services)
        {
            typename T::ProcessorType* processor = nullptr;
            for (auto& kv : ThriftMuxProcessor::m_Processors)
            {
                processor = dynamic_cast<typename T::ProcessorType*>(kv.second.get());
                if (processor)
                {
                    ThriftMuxProcessorTemplate<T>::m_CurProcessor = processor;
                    break;
                }

            }
        }

        virtual void process(std::unique_ptr<apache::thrift::ResponseChannel::Request> req,
            std::unique_ptr<folly::IOBuf> buf,
            apache::thrift::protocol::PROTOCOL_TYPES protType,
            apache::thrift::Cpp2RequestContext* context,
            folly::EventBase* eb,
            apache::thrift::concurrency::ThreadManager* tm) override
        {
            //since it's last processor to process, it will process anyway even if it's going to fail
            apache::thrift::detail::ap::process(ThriftMuxProcessorTemplate<T>::m_CurProcessor, std::move(req), std::move(buf), protType, context, eb, tm);
        }
    private:
        typename T::ProcessorType * m_CurProcessor;
    };

    class ThriftMuxService : public apache::thrift::ServerInterface
    {
        public:
            ThriftMuxService();
            virtual std::unique_ptr<apache::thrift::AsyncProcessor> getProcessor();  
            void addServiceInteface(std::shared_ptr<apache::thrift::ServerInterface> service);

        protected:
            //std::unique_ptr<ThriftMuxProcessor> m_MuxProcessor;
            std::vector<std::shared_ptr<apache::thrift::ServerInterface>> m_MuxServices;
    };

    


    template<class T, class... Ts>
    class ThriftMuxServiceTemplate : public ThriftMuxServiceTemplate<Ts...>
    {
    public:
        
        ThriftMuxServiceTemplate()
            : ThriftMuxServiceTemplate<Ts...>()
        {
            auto service = std::make_shared<T>();
            ThriftMuxService::addServiceInteface(service);
        }

        virtual std::unique_ptr<apache::thrift::AsyncProcessor> getProcessor()
        {
            return std::make_unique<ThriftMuxProcessorTemplate<T, Ts...>>(ThriftMuxService::m_MuxServices);
        }
    };

    template<class T>
    class ThriftMuxServiceTemplate<T> : public ThriftMuxService
    {
    public:
        ThriftMuxServiceTemplate<T>()
        {
            auto service = std::make_shared<T>();
            addServiceInteface(service);
        }

        virtual std::unique_ptr<apache::thrift::AsyncProcessor> getProcessor()
        {
            return std::make_unique<ThriftMuxProcessorTemplate<T>>(ThriftMuxService::m_MuxServices);
        }
    };
  
   

    
    


}