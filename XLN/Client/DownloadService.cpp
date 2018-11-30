//
//  DownloadService.cpp
//  LGCocosSample
//
//  Created by Rex Lien on 3/18/16.
//
//

#include "DownloadService.h"
#include <memory>

//#include "network/CCDownloader-apple.h"


#include "DownloaderEventCurl.h"


using namespace XGf;


XLN_IMPL_SERVICE_CLASS(DownloadService, XCr::Service);
DownloadService::DownloadService()
{
    //deafult, hints is possilbe to be overrided anyway
//    cocos2d::network::DownloaderHints hints =
//    {
//        3,
//        15,
//        ".tmp"
//    };
//    
      m_CDNDownloader = std::make_unique<DownloaderEventCurl>();
//    //m_CDNDownloader = new cocos2d::network::Downloader(std::unique_ptr<DownloaderCurl>(new DownloaderCurl(hints)));
//
//    //m_CDNDownloader = new cocos2d::network::Downloader(std::unique_ptr<cocos2d::network::DownloaderApple>(new DownloaderApple(hints)));
//    
//    m_CDNDownloader->onDataTaskSuccess = [this](const cocos2d::network::DownloadTask& task, std::vector<unsigned char>& data)
//    {
//        
//        if(m_CDNTaskSuccessCB)
//        {
//            m_CDNTaskSuccessCB(task, &data);
//        }
//        delete (DownloadService::RequestParamExt*)task.m_ExtraParam;
//        
//    };
//    
//    m_CDNDownloader->onTaskError = [this](const cocos2d::network::DownloadTask& task,
//                                          int errorCode,
//                                          int errorCodeInternal,
//                                          const std::string& errorStr)
//    {
//        if(m_CDNTaskFailCB)
//        {
//            m_CDNTaskFailCB(task, errorCode, errorCodeInternal, errorStr);
//        }
//
//        delete (DownloadService::RequestParamExt*)task.m_ExtraParam;
//        
//    };
//    
//    m_CDNDownloader->onFileTaskSuccess = [this](const cocos2d::network::DownloadTask& task)
//    {
//        if(m_CDNTaskSuccessCB)
//        {
//            m_CDNTaskSuccessCB(task, 0);
//        }
//
//        delete (DownloadService::RequestParamExt*)task.m_ExtraParam;
//    };
//
//    
}



void DownloadService::downloadToFile(const std::string& url, const std::string& fileName, uint32_t timeOut)
{
    
    //NOTE: downloader does not cache, so make sure to check if there's cache file with latest version existed in given filename;
//    std::shared_ptr<LG::Core::Timer> timer = std::make_shared<LG::Core::Timer>();
//    network::DownloaderHints hints =
//    {
//        1,
//        timeOut,
//        ".tmp"
//    };
//    
//    network::Downloader* downloader = new network::Downloader(hints);
//    
//    downloader->onDataTaskSuccess = [this, downloader, timer](const network::DownloadTask& task, std::vector<unsigned char>& data)
//    {
//        timer->printLog(std::string("Download Success:") + task.requestURL);
//        m_DownloaderToDelete.push_back(downloader);
//        
//    };
//    
//    downloader->onTaskError = [this, downloader, timer](const network::DownloadTask& task,
//                                                        int errorCode,
//                                                        int errorCodeInternal,
//                                                        const std::string& errorStr)
//    {
//        
//        timer->printLog(std::string("Download Failed:") + task.requestURL);
//        m_DownloaderToDelete.push_back(downloader);
//    };
//    
//    downloader->onFileTaskSuccess = [this, downloader, timer](const network::DownloadTask& task)
//    {
//        
//        timer->printLog(std::string("Download Success:") + task.requestURL);
//        m_DownloaderToDelete.push_back(downloader);
//    };
//    
//    
//    timer->start();
//    downloader->createDownloadFileTask(url, fileName);
    
    
}

void DownloadService::CDNRequest(const std::string& url, uint32_t maxConcurrency, uint32_t dnsCacheTime, bool compressionOn, uint32_t timeout, DownloadService::HTTPVersion httpVersion, bool reuseConnection
             , DownloadService::TaskSuccessCB successCB, DownloadService::TaskFailCB failCB)
{
    
//    std::shared_ptr<DownloadTask> s = std::make_shared<DownloadTask>();
//    s->requestURL = url;
//    s->m_CompressionOn = compressionOn;
//    s->m_DNSCacheTime = dnsCacheTime;
//    s->m_HttpVersion = httpVersion;
//    s->m_Timeout = timeout;
//    DownloadService::RequestParamExt* extraParam = new DownloadService::RequestParamExt();
//    extraParam->m_RequestID = m_RequestID++;
//    extraParam->m_ReuseConneciton = reuseConnection;
//    s->m_ExtraParam = extraParam;
//    
//    //s->m_ExtraParam->m_Timer
//    m_CDNDownloader->sendDownloadTask(s);

}

folly::Future<std::shared_ptr<CurlDownloadTask> > DownloadService::RequestCDN(const std::string &url,
                                                                     uint32_t maxConcurrency,
                                                                     uint32_t dnsCacheTime,
                                                                     bool compressionOn,
                                                                     uint32_t timeout,
                                                                     DownloadService::HTTPVersion httpVersion,
                                                                     bool reuseConnection) {

    folly::Promise<std::shared_ptr<CurlDownloadTask>> promise;
    //std::shared_ptr<Response> response(new Response());//= std::make_shared<DownloadService::Response>();
    //response->m_ErrorCode = 10000;

    //promise.setValue(response);

    //std::shared_ptr<CurlDownloadTask> s = std::make_shared<CurlDownloadTask>();
    CurlDownloadTask* s = new CurlDownloadTask();
    s->m_RequestURL = url;
    s->m_CompressionOn = compressionOn;
    s->m_DNSCacheTTL = dnsCacheTime;
    s->m_HTTPVersion = httpVersion;
    s->m_Timeout = timeout;
    s->m_ReuseConneciton = reuseConnection;
    //CurlDownloadTask::RequestParamExt* extraParam = new CurlDownloadTask::RequestParamExt();
    s->m_RequestParamExt.m_RequestID = m_RequestID++;

    //extraParam->m_ReuseConneciton = reuseConnection;
    //s->m_RequestParamExt = extraParam;
    s->m_Promise = std::move(promise);

    m_CDNDownloader->sendDownloadTask(s);

    return s->m_Promise.getFuture();//folly::makeFuture();


}


void DownloadService::setCDNRequestCB(TaskSuccessCB successCB, TaskFailCB failCB )
{
    m_CDNTaskSuccessCB = successCB;
    m_CDNTaskFailCB = failCB;
}

void DownloadService::setCDNMaxConcurrency(int maxConquerrency)
{
    //m_CDNDownloader->setMaxConcurrecy(maxConquerrency);
}


void DownloadService::update()
{
	cleanDownloader();
}

void DownloadService::cleanDownloader()
{
//    std::list<cocos2d::network::Downloader*>::iterator iter(m_DownloaderToDelete.begin());
//    while(iter != m_DownloaderToDelete.end())
//    {
//    	delete (*iter);
//    	++iter;
//    }
//    m_DownloaderToDelete.clear();
}

void DownloadService::ResetRequestID()
{
    m_RequestID = 0;
}

bool DownloadService::OnTick(float delta) {
    XCr::Service::OnTick(delta);
    m_CDNDownloader->Tick(delta);
    return true;
}

