//
//  Task.hpp
//  LGCocosSample
//
//  Created by Rex Lien on 8/17/16.
//
//

#ifndef Task_hpp
#define Task_hpp

#include <XLN/Core/Config/Config.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <functional>
#include "curl/curl.h"
#include "Timer.h"
#include <folly/futures/Future.h>

namespace XGf {

/*
class Response
{
public:
    int m_ErrorCode;
    char m_CurlErrbuf[CURL_ERROR_SIZE];
    std::string m_ErrorString;
    size_t m_ResBodyLength;

};
*/

class Task
{
public:
    virtual ~Task() = 0;
    
    virtual void Run() {};
};
    
class RequestTask: public Task
{
public:
    ~RequestTask() {};
    std::string m_RequestURL;
    int m_Timeout;
    bool m_CompressionOn;
    int m_DNSCacheTTL;
    int m_HTTPVersion;
    bool m_ReuseConneciton;
    //folly::Promise<std::shared_ptr<Response>> m_Promise;
};

class DownloadTask : public RequestTask
{
public:
    ~DownloadTask() {};
    typedef std::function<void(const DownloadTask& task, std::vector<unsigned char>* data)> TaskSuccessCB;
    typedef std::function<void(const DownloadTask& task, int errorCode, int errorCodeInternal, const std::string& errorStr)> TaskFailCB;
    
    TaskSuccessCB m_SuccessCB;
    TaskFailCB m_FailCB;

};
    
class CurlDownloadTask : public DownloadTask// : public cocos2d::network::IDownloadTask
{
public:
    
    class RequestParamExt
    {
        
    public:
        std::vector<std::vector<char>> m_Headers;
        std::shared_ptr<LG::Core::Timer> m_Timer;
        int m_RequestID;
        std::string m_DestIP;
        int m_SrcPort;
        double m_SizeDownloaded;
        int m_CurlNameLookupTime;
        int m_CurlConnetionTime;
        int m_CurlPretransferTime;
        int m_CurlFirstTransferTime;
        int m_CurlTransferTime;
        int m_CurlTotalTime;
        
        
        RequestParamExt()
        {
            m_Timer =  std::make_shared<LG::Core::Timer>();
        }
        
        ~RequestParamExt()
        {
            
        }
        
        
    };

    
    CurlDownloadTask();
    ~CurlDownloadTask();
    
    //std::shared_ptr<const cocos2d::network::DownloadTask> m_CoTask;
    int m_ErrorCode;
    char m_CurlErrbuf[CURL_ERROR_SIZE];
    std::string m_ErrorString;
    size_t m_ResBodyLength;
    
    RequestParamExt m_RequestParamExt;
    folly::Promise<std::shared_ptr<CurlDownloadTask>> m_Promise;
    
};

}



#endif /* Task_hpp */
