//
//  NetworkManager.hpp
//  LGCocosSample
//
//  Created by Rex Lien on 3/18/16.
//
//

#ifndef __XLN_NetworkManager_h__
#define __XLN_NetworkManager_h__

#include <stdio.h>
#include <stdint.h>
#include <string>
#include <list>
#include <atomic>
#include <memory>
#include <vector>
#include <functional>


#include <XLN/Core/Config/Config.h>

#include <XLN/Core/Foundation/Service.h>
#include <folly/futures/Future.h>
#include "DownloaderEventCurl.h"
#include "Timer.h"
#include "Task.h"



namespace XGf
{
    
class XLN_CLASS_DECL DownloadService : public XCr::Service
{

    XLN_DECLARE_SERVICE_CLASS(DownloadService)


public:


    class DownloaderHint
    {
        
    };
    
    
    enum HTTPVersion
    {
        HTTP1_1,
        HTTP2
    };

    
    class DefRetryPolicy
    {
    public:
        static uint32_t getNextRetryTimeOut(uint32_t curTimeOut)
        {
            //double previous timeout for next retry
            return curTimeOut * 2;
        }
        static bool shouldGiveupRetry(uint32_t curRetryCount)
        {
            //only retry twice
            return (curRetryCount >= 2);
        }
    };
    
    //static NetworkManager& getInstance();

    DownloadService();
    ~DownloadService()
    {
        //delete m_CDNDownloader;
    }

    virtual bool OnTick(float delta) override;;
    void downloadToFile(const std::string& url, const std::string& fileName, uint32_t timeOut);
    
    template<class RetryPolicy = DefRetryPolicy>
    void downloadWithRetry(const std::string& url, const std::string& fileName, uint32_t timeOut)
    {
        downloadWithRetry<RetryPolicy>(url, fileName, timeOut, 0);
    }
    
    typedef std::function<void(const CurlDownloadTask& task, std::vector<unsigned char>* data)> TaskSuccessCB;
    typedef std::function<void(const CurlDownloadTask& task, int errorCode, int errorCodeInternal, const std::string& errorStr)> TaskFailCB;
    
    void CDNRequest(const std::string& url, uint32_t maxConcurrency, uint32_t dnsCacheTime, bool compressionOn, uint32_t timeout, HTTPVersion httpVersion=HTTP1_1, bool reuseConnection = true
                    , TaskSuccessCB successCB = 0, TaskFailCB failCB = 0);

    folly::Future<std::shared_ptr<CurlDownloadTask> > RequestCDN(const std::string& url, uint32_t maxConcurrency, uint32_t dnsCacheTime, bool compressionOn, uint32_t timeout, HTTPVersion httpVersion=HTTP1_1, bool reuseConnection = true);
    
    void setCDNMaxConcurrency(int maxConquerrency);
    void setCDNRequestCB(TaskSuccessCB successCB, TaskFailCB failCB );
    //call this every frame
    void update();
    void ResetRequestID();
    
    TaskSuccessCB m_CDNTaskSuccessCB;
    TaskFailCB m_CDNTaskFailCB;
    int m_CountOfMaxProcessingTasks;

private:

    std::unique_ptr<DownloaderEventCurl> m_CDNDownloader;

    std::atomic<int> m_RequestID;
    

    template<class RetryPolicy = DefRetryPolicy>
    void downloadWithRetry(const std::string& url, const std::string& fileName, uint32_t curTimeOut, uint32_t curRetryCount)
    {

        
               
    }

    void cleanDownloader();
 
    
    
    
};
    
}



#endif /* NetworkManager_hpp */
