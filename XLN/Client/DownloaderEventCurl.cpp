#include "DownloaderEventCurl.h"
#include "curl/curl.h"
#include "DownloadService.h"
#include <event2/thread.h>
#include <functional>

using namespace XGf;
//using namespace LG::Core;

CurlPool DownloaderEventCurl::m_CurlPool;

static size_t bodyCallbackProc(void *buffer, size_t size, size_t count, void *userdata)
{
    CurlDownloadTask* task = (CurlDownloadTask*)userdata;
    size_t totalSize = size * count;
    task->m_ResBodyLength += totalSize;
    
    return totalSize;
}

static size_t headerCallbackProc(void *buffer, size_t size, size_t count, void *userdata)
{
    
    // std::vector<char> str;
    //str.resize(size*count + 1);
    //memcpy(&str[0], buffer, size*count);
    //str[str.size() - 1] = '\0';
    
    return size*count;
}



void DownloaderEventCurl::OnInterrupt(evutil_socket_t sock, short which, void *arg)
{
    DownloaderEventCurl* downloader = (DownloaderEventCurl*)arg;
    
    CurlDownloadTask* waitingTask = 0;
    while( (downloader->m_CountOfMaxProcessingTasks == 0 || downloader->m_RunningTasks.size() < downloader->m_CountOfMaxProcessingTasks) && downloader->m_WaitingTask->pop(waitingTask) )
    {
        if(waitingTask)
        {
            
            CURL* handle = m_CurlPool.Obtain();
            downloader->SetCurl(handle, *waitingTask);
            CurlDownloadTask::RequestParamExt* paramExt = (CurlDownloadTask::RequestParamExt* )&waitingTask->m_RequestParamExt;
            
            curl_multi_add_handle(downloader->m_CurlMHandle, handle);
            downloader->m_RunningTasks.insert(std::make_pair(handle, waitingTask));
            paramExt->m_Timer->start();
        }
    }
    
    
    
}

void DownloaderEventCurl::OnExit(evutil_socket_t sock, short which, void *arg)
{
    DownloaderEventCurl* downloader = (DownloaderEventCurl*)arg;
    event_base_loopbreak(downloader->m_EventBase);
}




DownloaderEventCurl::DownloaderEventCurl()
{
#ifdef _WIN32
	evthread_use_windows_threads();
#else
    evthread_use_pthreads();
#endif
    m_WaitingTask = new boost::lockfree::queue<DownloadTask*>(32);
    //m_FinishedTask = new boost::lockfree::queue<DownloadTask*>(32);
    
    m_EventBase = event_base_new();
    event_base_priority_init(m_EventBase, 2);
    m_InteruptEvent = event_new(m_EventBase, -1, EV_PERSIST | EV_READ, OnInterrupt, this);
    event_priority_set(m_InteruptEvent, 0);
    event_add(m_InteruptEvent, NULL);
    
    m_ExitEvent = event_new(m_EventBase, -1, EV_PERSIST | EV_READ, OnExit, this);
    event_priority_set(m_ExitEvent, 0);
    event_add(m_ExitEvent, NULL);

    
    m_CurlTimerEvent = evtimer_new(m_EventBase, OnTimer, this);
    event_priority_set(m_CurlTimerEvent, 0);

    //Curl_share* share = curl_share_init();
    //curl_share_setopt(share, CURLSHOPT_SHARE, CURL_LOCK_DATA_DNS);

    
    if (!m_Thread.joinable())
    {
        std::thread newThread(&DownloaderEventCurl::ThreadProc, this);
        m_Thread.swap(newThread);
    }
 
}

void DownloaderEventCurl::SetCurl(CURL* handle, const CurlDownloadTask& downloadTask)
{
    CurlDownloadTask::RequestParamExt* paramExt = (CurlDownloadTask::RequestParamExt*)&(downloadTask.m_RequestParamExt);
    //curl_easy_setopt(handle, CURLOPT_VERBOSE, 1L);
    if(!downloadTask.m_ReuseConneciton)
    {
        curl_easy_setopt(handle, CURLOPT_FRESH_CONNECT, 1);
        curl_easy_setopt(handle, CURLOPT_FORBID_REUSE, 1);
    }
    
    curl_easy_setopt(handle, CURLOPT_ERRORBUFFER, downloadTask.m_CurlErrbuf);
    
    curl_easy_setopt(handle, CURLOPT_TCP_NODELAY, 1);
    curl_easy_setopt(handle, CURLOPT_URL, downloadTask.m_RequestURL.c_str());
    
    // set write func
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, bodyCallbackProc);
    curl_easy_setopt(handle, CURLOPT_HEADERFUNCTION, headerCallbackProc);
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, &downloadTask);
    curl_easy_setopt(handle, CURLOPT_HEADERDATA, &downloadTask);
    
    //curl_easy_setopt(handle, CURLOPT_NOPROGRESS, true);
    //            curl_easy_setopt(handle, CURLOPT_XFERINFOFUNCTION, DownloaderCURL::Impl::_progressCallbackProc);
    //            curl_easy_setopt(handle, CURLOPT_XFERINFODATA, coTask);
    
    curl_easy_setopt(handle, CURLOPT_FAILONERROR, true);
    curl_easy_setopt(handle, CURLOPT_NOSIGNAL, 1L);
    
    curl_easy_setopt(handle, CURLOPT_CONNECTTIMEOUT, downloadTask.m_Timeout);
    
    
    static const long LOW_SPEED_LIMIT = 1024 * 5; //byte per second
    curl_easy_setopt(handle, CURLOPT_LOW_SPEED_LIMIT, LOW_SPEED_LIMIT);
    curl_easy_setopt(handle, CURLOPT_LOW_SPEED_TIME, downloadTask.m_Timeout);
    
    static const int MAX_REDIRS = 2;
    if (MAX_REDIRS)
    {
        curl_easy_setopt(handle, CURLOPT_FOLLOWLOCATION, true);
        curl_easy_setopt(handle, CURLOPT_MAXREDIRS, MAX_REDIRS);
    }
    
    if(downloadTask.m_CompressionOn)
        curl_easy_setopt(handle, CURLOPT_ACCEPT_ENCODING, "");
    else
        curl_easy_setopt(handle, CURLOPT_ACCEPT_ENCODING, NULL);
    
    curl_easy_setopt(handle, CURLOPT_DNS_CACHE_TIMEOUT, downloadTask.m_DNSCacheTTL);
    
    
    if(downloadTask.m_HTTPVersion == 0)
    {   curl_easy_setopt(handle, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_1_1);
        curl_easy_setopt (handle, CURLOPT_SSL_VERIFYHOST, 0);
        curl_easy_setopt (handle, CURLOPT_SSL_VERIFYPEER, 0);
    }
    else
    {   curl_easy_setopt(handle, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_2_0);
        curl_easy_setopt (handle, CURLOPT_SSL_VERIFYHOST, 0);
        curl_easy_setopt (handle, CURLOPT_SSL_VERIFYPEER, 0);
        curl_easy_setopt(handle, CURLOPT_PIPEWAIT, 1);
    }
    
    
}


void DownloaderEventCurl::OnCurlTimer(CURLM *multi, long timeout_ms, DownloaderEventCurl* param)
{
    struct timeval timeout;
    (void)multi; /* unused */
    int still_running;
    
    timeout.tv_sec = timeout_ms/1000;
    timeout.tv_usec = (timeout_ms%1000)*1000;
    /*
     if(timeout_ms == 0)
     {
     curl_multi_socket_action(param->m_CurlMHandle,
     CURL_SOCKET_TIMEOUT, 0, &still_running);
     CheckSocket(param);
     
     
     }*/
    if(timeout_ms == -1)
    {
        evtimer_add(param->m_CurlTimerEvent, NULL);
    }
    else
        evtimer_add(param->m_CurlTimerEvent, &timeout);
}

void DownloaderEventCurl::OnTimer(evutil_socket_t fd, short kind, void *param)
{
    DownloaderEventCurl* downloader = (DownloaderEventCurl*)param;
    
    CURLMcode rc;
    (void)fd;
    (void)kind;
    int still_running;
    
    rc = curl_multi_socket_action(downloader->m_CurlMHandle,
                                  CURL_SOCKET_TIMEOUT, 0, &still_running);
    
    CheckSocket(downloader);
}

void DownloaderEventCurl::OnSocketEvent(evutil_socket_t fd, short kind, void* param)
{
    DownloaderEventCurl* downloader = (DownloaderEventCurl*)param;
    
    CURLMcode rc;
    (void)fd;
    (void)kind;
    int still_running;
    
    rc = curl_multi_socket_action(downloader->m_CurlMHandle,
                                  fd, 0, &still_running);
    
    CheckSocket(downloader);
    
}

void DownloaderEventCurl::CheckSocket(void* param)
{
    DownloaderEventCurl* downloader = (DownloaderEventCurl*)param;
    
    int msgs_left;
    CURLMsg *msg;
    //curl_multi_perform(downloader->m_CurlMHandle, &still_running);
    bool hasMsg = false;
    
    while((msg = curl_multi_info_read(downloader->m_CurlMHandle, &msgs_left)))
    {
        hasMsg = true;
        if(msg->msg == CURLMSG_DONE)
        {
            CURL* handle = msg->easy_handle;
            
            std::map<CURL*, DownloadTask*>::iterator iter(downloader->m_RunningTasks.find(handle));
            
            if(iter != downloader->m_RunningTasks.end())
            {
                CurlDownloadTask* task = (CurlDownloadTask*)iter->second;
                
                //stop timer
                CurlDownloadTask::RequestParamExt* paramExt = (CurlDownloadTask::RequestParamExt* )&(task->m_RequestParamExt);
                paramExt->m_Timer->stop();
                
                paramExt->m_SizeDownloaded = 0;
                
                char* ip = 0;
                CURLcode rc = curl_easy_getinfo(handle, CURLINFO_PRIMARY_IP, &ip);
                if(rc == CURLE_OK && ip)
                {
                    paramExt->m_DestIP = ip;
                }
                
                long port = 0;
                rc = curl_easy_getinfo(handle, CURLINFO_LOCAL_PORT, &port);
                if(rc == CURLE_OK)
                {
                    paramExt->m_SrcPort = (int)port;
                }
                
                double dVal = 0;
                rc = curl_easy_getinfo(handle, CURLINFO_SIZE_DOWNLOAD, &dVal);
                if(rc == CURLE_OK)
                {
                    paramExt->m_SizeDownloaded = dVal;
                }
                
                double nameLookupTime;
                curl_easy_getinfo(handle, CURLINFO_NAMELOOKUP_TIME, &nameLookupTime);
                paramExt->m_CurlNameLookupTime = nameLookupTime * 1000;
                
                
                double connectionTime;
                curl_easy_getinfo(handle, CURLINFO_CONNECT_TIME, &connectionTime);
                paramExt->m_CurlConnetionTime = (connectionTime - nameLookupTime) * 1000;
                
                
                double appConnectTime;
                rc = curl_easy_getinfo(handle, CURLINFO_APPCONNECT_TIME, &appConnectTime);
                
                
                double preTransferTime;
                curl_easy_getinfo(handle, CURLINFO_PRETRANSFER_TIME, &preTransferTime);
                paramExt->m_CurlPretransferTime = (preTransferTime - appConnectTime) * 1000;
                
                double startTransferTime;
                curl_easy_getinfo(handle, CURLINFO_STARTTRANSFER_TIME, &startTransferTime);
                paramExt->m_CurlFirstTransferTime = (startTransferTime - preTransferTime) * 1000;
                
                double totalTime;
                curl_easy_getinfo(handle, CURLINFO_TOTAL_TIME, &totalTime);
                paramExt->m_CurlTotalTime = totalTime * 1000;
                
                paramExt->m_CurlTransferTime = (totalTime - preTransferTime) * 1000;
                
                if(msg->data.result != CURLE_OK)
                {
                    task->m_ErrorCode = -1;
                    
                    if(strlen(task->m_CurlErrbuf) != 0)
                    {
                         task->m_ErrorString  = task->m_CurlErrbuf;
                    }
                    else
                    {   const char *errorStr = curl_easy_strerror(msg->data.result);
                        if(errorStr)
                        {
                            task->m_ErrorString  = errorStr;
                        }
                        else
                        {
                            task->m_ErrorString = "Curl Unknown Error";
                        }
                    }
                    
                }
                else
                {
                    long httpResponseCode = 0;
                    rc = curl_easy_getinfo(handle, CURLINFO_RESPONSE_CODE, &httpResponseCode);
                    if (CURLE_OK != rc)
                    {
                        task->m_ErrorCode = -1;
                        task->m_ErrorString = "Curl Internal Error";
                        
                    }
                    else
                    {
                        if (httpResponseCode < 300 && httpResponseCode >= 200)
                        {
                            task->m_ErrorCode = 0;
                        }
                        else
                        {
                            task->m_ErrorCode = (int)httpResponseCode;
                            char buf[256];
                            sprintf(buf, "Http Error:%d", (int)task->m_ErrorCode);
                            task->m_ErrorString = buf;
                        }
                    }
                    
                }
                //std::shared_ptr<XGf::Response> resp = std::make_shared<XGf::Response>();

                task->m_Promise.setValue(std::shared_ptr<CurlDownloadTask>(task));
                //downloader->m_FinishedTask->push(iter->second);
                downloader->m_RunningTasks.erase(iter);
            }
            curl_multi_remove_handle(downloader->m_CurlMHandle, handle);
            m_CurlPool.Return(handle);
            
        }
    }
    
    if(hasMsg)
    {
        event_active(downloader->m_InteruptEvent, EV_WRITE, 0);
        
    }
}

void DownloaderEventCurl::AddSocketEvent(event_base* evBase, SockInfo* info, int act)
{
    int kind = (act&CURL_POLL_IN?EV_READ:0)|(act&CURL_POLL_OUT?EV_WRITE:0)|EV_PERSIST;
    
    if(info->m_Event)
    {
        event_free(info->m_Event);
    }
    info->m_Event = event_new(evBase, info->m_Socket, kind, DownloaderEventCurl::OnSocketEvent, info->m_Downloader);
    event_add(info->m_Event, 0);
}

void DownloaderEventCurl::RemoveSocketEvent(SockInfo* info)
{
    if(!info)
        return;
    
    if(info->m_Event)
    {
        event_free(info->m_Event);
    }
    delete info;
}


int DownloaderEventCurl::CurlSocketCB(CURL *e, curl_socket_t s, int what, void *cbp, void *sockp)
{
    SockInfo* info = (SockInfo*)sockp;
    DownloaderEventCurl* downloader = (DownloaderEventCurl*)cbp;
    
    if(what == CURL_POLL_REMOVE)
    {
        RemoveSocketEvent(info);
    }
    else
    {
        
        SockInfo *newInfo;
        if(!info)
        {
            newInfo = new SockInfo();
            /*
            int curBufSize = 0;
            socklen_t len = 4;
            getsockopt(s, SOL_SOCKET, SO_RCVBUF, &curBufSize, &len);
            curBufSize*=4;
            setsockopt(s, SOL_SOCKET, SO_RCVBUF, &curBufSize, sizeof(int));
*/
            curl_multi_assign(downloader->m_CurlMHandle, s, newInfo);
        }
        else
        {
            newInfo = info;
        }
        
        newInfo->m_Socket = s;
        newInfo->m_Easy = e;
        newInfo->m_Downloader = downloader;
        AddSocketEvent(downloader->m_EventBase, newInfo, what);
        
    }
    
    return 0;
}

void DownloaderEventCurl::ThreadProc()
{
    
    m_CurlMHandle = curl_multi_init();
    
    curl_multi_setopt(m_CurlMHandle, CURLMOPT_PIPELINING, CURLPIPE_MULTIPLEX);
    curl_multi_setopt(m_CurlMHandle, CURLMOPT_SOCKETFUNCTION, CurlSocketCB);
    curl_multi_setopt(m_CurlMHandle, CURLMOPT_SOCKETDATA, this);
    curl_multi_setopt(m_CurlMHandle, CURLMOPT_TIMERFUNCTION, OnCurlTimer);
    curl_multi_setopt(m_CurlMHandle, CURLMOPT_TIMERDATA, this);
    
  
    event_base_dispatch(m_EventBase);
    
    curl_multi_cleanup(m_CurlMHandle);
    m_CurlMHandle = 0;
}


DownloaderEventCurl::~DownloaderEventCurl()
{
    event_active(m_ExitEvent, EV_WRITE, 0);
    if(m_Thread.joinable())
    {
        m_Thread.join();
    }
    
    //m_Scheduler->unschedule(m_SchedulerKey, this);
    //m_Scheduler->release();

    delete m_WaitingTask;
    //delete m_FinishedTask;
    
    event_free(m_InteruptEvent);
    event_free(m_CurlTimerEvent);
    event_free(m_ExitEvent);
    event_base_free(m_EventBase);
}


void DownloaderEventCurl::Tick(float time)
{
    std::list<CurlDownloadTask*> processingTask;
    
    CurlDownloadTask* finishedTask = 0;
    /*
    while(m_FinishedTask->pop(finishedTask))
    {
        if(finishedTask)
        {
            
            std::vector<unsigned char> data;
            //onTaskFinish(*finishedTask->m_CoTask, finishedTask->m_ErrorCode, finishedTask->m_ErrorCode, finishedTask->m_ErrorString, data);
            
            delete finishedTask;
        }
    }
     */
}

void DownloaderEventCurl::sendDownloadTask(DownloadTask* task)
{
    m_WaitingTask->push(task);
    event_active(m_InteruptEvent, EV_WRITE, 0);
}


/*
IDownloadTask *DownloaderEventCurl::createCoTask(std::shared_ptr<const DownloadTask>& task)
{
    m_WaitingTask->push(new CurlDownloadTask(task));
    event_active(m_InteruptEvent, EV_WRITE, 0);
    
    return 0;
}
*/
void DownloaderEventCurl::setMaxConcurrency(int maxConcurrency)
{
    m_CountOfMaxProcessingTasks = maxConcurrency;
}