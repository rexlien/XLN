#include "ThriftApplication.h"

#include <thrift/lib/cpp2/server/ThriftServer.h>
#include <XLN/Core/Foundation/Log.h>
#include <XLN/Core/Framework/AssetService.h>
#include <XLN/Core/Framework/ConfigService.h>
#include <XLN/Core/Foundation/ServiceMgr.h>
#include <XLN/Core/Utility/Base64.h>
#include <folly/FBString.h>
#include <boost/algorithm/string.hpp>
#include <openssl/sha.h>
#include <nlohmann/json.hpp>

using namespace XLN;
using namespace apache::thrift;

static const char* const CRLF = "\r\n";

ThriftApplication::ThriftApplication(std::shared_ptr<apache::thrift::AsyncProcessorFactory> processorFactory)
    : m_ProcessorFactory(processorFactory), m_AcceptWebSocket(false)
{
    
}

static void WebSocketHttpHandlerFunc(folly::EventBase* base,
    std::shared_ptr<apache::thrift::async::TAsyncTransport> transport,
    std::unique_ptr<folly::IOBuf> iobuf, bool *keepChannel)
{

    folly::fbstring string = iobuf->moveToFbString();
    std::vector<std::string> v;
    boost::split(v, string, boost::is_any_of("\n"));
    std::string secretKey;
    for(auto& e : v)
    {
        if(boost::algorithm::starts_with(e, "Sec-WebSocket-Key"))//e.find("Sec-WebSocket-Key") == 0)
        {
            boost::split(v, e, boost::is_any_of(":"));
            if(v.size() > 1)
            {   boost::trim(v[1]); 
                secretKey = v[1];
                break;
            }
            else
            {
                return;
            }           
        }
    
    }

    //encode accept key
    secretKey += "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
    std::vector<unsigned char> obuf;
    obuf.resize(20);
    SHA1((const unsigned char*)(secretKey.c_str()), secretKey.size(), &obuf[0]);
    std::string result;
    XCr::base64::encode(obuf.begin(), obuf.end(), std::back_inserter(result));


    std::ostringstream h;
    h <<
    "HTTP/1.1 101 Switching Protocols" << CRLF <<
    "Upgrade: websocket" << CRLF <<
    "Connection: Upgrade" << CRLF <<
    "Sec-WebSocket-Accept: " << result << CRLF <<
    CRLF;
    std::string header = h.str();

    // Write the header, then the data, then flush
    transport->write(0, (const uint8_t*)header.c_str(), header.size());
    *keepChannel = true;
  
}

void XLN::ThriftApplication::StartServer(int argc, char ** argv)
{
    XLN_LOGD("Starting Server");

    /*
    XGf::AssetService* assetService = XCr::ServiceMgr::GetActiveServiceMgr()->GetService<XGf::AssetService>();
    nlohmann::json json = nullptr;
    try
    {
        std::ifstream fstream(assetService->ResolveFilePath("Asset", "XLNConfig.json"), std::ifstream::in);
        json = nlohmann::json::parse(fstream);
      
    }
    catch (std::exception ex)
    {
        XLN_LOGE("Config load error: %s", ex.what());
        throw ex;
    }
    
    */
    XGf::ConfigService* configService = XCr::ServiceMgr::GetActiveServiceMgr()->GetService<XGf::ConfigService>();

    auto thriftServer = std::make_shared<apache::thrift::ThriftServer>();
    thriftServer->setNumIOWorkerThreads(configService->GetAppConfig()["XLNAppConfig"]["ServerConfigs"]["ThriftServer"]["IOThreadCount"]);
    thriftServer->setNumCPUWorkerThreads(configService->GetAppConfig()["XLNAppConfig"]["ServerConfigs"]["ThriftServer"]["CPUThreadCount"]);
    thriftServer->setIdleTimeout(std::chrono::milliseconds(configService->GetAppConfig()["XLNAppConfig"]["ServerConfigs"]["ThriftServer"]["IdleTimeout"]));
    
    int port = configService->GetAppConfig()["XLNAppConfig"]["ServerConfigs"]["ThriftServer"]["AcceptPort"];
    thriftServer->setPort(port);
    thriftServer->setProcessorFactory(m_ProcessorFactory);
    if(m_AcceptWebSocket)
        thriftServer->setGetHandler(WebSocketHttpHandlerFunc);

    m_ThriftThread.start(thriftServer);
}

 void XLN::ThriftApplication::StopServer()
 {

     XLN_LOGD("Stopping Server");
     m_ThriftThread.stop();

 }

 void XLN::ThriftApplication::EnableAcceptWebScoket(bool accept)
 {
     m_AcceptWebSocket = accept;
 }
