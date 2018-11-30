//
//  DownloaderEventCurl.hpp
//  LGCocosSample
//
//  Created by Rex Lien on 8/15/16.
//
//

#pragma once

#include <XLN/Core/Config/Config.h>
#include "curl/curl.h"
#include <thread>
#include <condition_variable>
#include "event2/event.h"
#include "CurlPool.h"
#include "Task.h"
#include <map>
//#include "tbb/concurrent_queue.h"


namespace XGf
{

class XLN_CLASS_DECL DownloaderEventCurl
{
public:
    DownloaderEventCurl();
    virtual ~DownloaderEventCurl();
    
    virtual void sendDownloadTask(DownloadTask* task);
    virtual void setMaxConcurrency(int maxConcurrency);
    void Tick(float time);
    
        
private:
    
    class SockInfo
    {
    public:
        curl_socket_t m_Socket;
        CURL *m_Easy;
        event *m_Event;
        DownloaderEventCurl* m_Downloader;
        
    };
    
    
    

    void ThreadProc();
    void SetCurl(CURL* handle, const CurlDownloadTask& downloadTask);
    
    static void CheckSocket(void* param);
    static void OnCurlTimer(CURLM *multi, long timeout_ms, DownloaderEventCurl* param);
    static void OnTimer(evutil_socket_t fd, short kind, void *param);
    static void OnSocketEvent(evutil_socket_t fd, short kind, void* param);
    static void AddSocketEvent(event_base* evBase, SockInfo* info, int act);
    static void RemoveSocketEvent(SockInfo* info);
    static void OnInterrupt(evutil_socket_t sock, short which, void *arg);
    static void OnExit(evutil_socket_t sock, short which, void *arg);
    
    static int CurlSocketCB(CURL *e, curl_socket_t s, int what, void *cbp, void *sockp);
    
    
    static CurlPool m_CurlPool;
    
    
    //cocos2d::Scheduler* m_Scheduler;
    CURLM* m_CurlMHandle;
    std::string m_SchedulerKey;
    //cocos2d::network::DownloaderHints m_Hints;
    
    boost::lockfree::queue<DownloadTask*> *m_WaitingTask;
    //boost::lockfree::queue<DownloadTask*> *m_FinishedTask;
    std::map<CURL*, DownloadTask*> m_RunningTasks;
    
    std::thread m_Thread;
    
    event* m_InteruptEvent;
    event* m_ExitEvent;
    event* m_CurlTimerEvent;
    event_base* m_EventBase;
    
    int m_CountOfMaxProcessingTasks;
    

    
    
};

}
