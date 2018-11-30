#include "ThriftMuxService.h"

using namespace XLN;

XLN::ThriftMuxProcessor::ThriftMuxProcessor(const std::vector<std::shared_ptr<apache::thrift::ServerInterface>>& services)
    : m_Services(services)
{
    for (auto& v : m_Services)
    {
        std::unique_ptr<apache::thrift::AsyncProcessor> processor = v->getProcessor();
        apache::thrift::GeneratedAsyncProcessor *gPrcoessor = dynamic_cast<apache::thrift::GeneratedAsyncProcessor *>(processor.get());
        m_Processors.insert(std::make_pair((gPrcoessor) ? gPrcoessor->getServiceName() : "", std::move(processor)));

    }
}

void XLN::ThriftMuxProcessor::process(std::unique_ptr<apache::thrift::ResponseChannel::Request> req, std::unique_ptr<folly::IOBuf> buf, apache::thrift::protocol::PROTOCOL_TYPES protType, apache::thrift::Cpp2RequestContext * context, folly::EventBase * eb, apache::thrift::concurrency::ThreadManager * tm)
{

}

bool XLN::ThriftMuxProcessor::isOnewayMethod(const folly::IOBuf * buf, const apache::thrift::transport::THeader * header)
{
    for (auto& kv : m_Processors)
    {
        if (kv.second->isOnewayMethod(buf, header))
        {
            return true;
        }
    }
    return false;
}

XLN::ThriftMuxService::ThriftMuxService()
{

}

std::unique_ptr<apache::thrift::AsyncProcessor> XLN::ThriftMuxService::getProcessor()
{
    return std::make_unique<ThriftMuxProcessor>(m_MuxServices);
}

void XLN::ThriftMuxService::addServiceInteface(std::shared_ptr<apache::thrift::ServerInterface> service)
{
    m_MuxServices.push_back(service);
}
