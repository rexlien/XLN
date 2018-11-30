#include "TaskThread.h"

#include "TaskRunner.h"
#include <XLN/Core/Foundation/Task/TaskConsumer.h>
#include "TaskQueue.h"

using namespace XCr;

XCr::TaskThread::TaskThread()
{
    m_Runnable = XLN_OBJ_NEW TaskConsumer();
}

TaskThread::TaskThread(TaskRunner *runner)
{
    m_Runnable = runner;
}

XCr::TaskThread::~TaskThread()
{

    Stop();
    
}


bool XCr::TaskThread::SetPriority( ThreadPriotiy priority )
{
    //if(!m_Thread)
    //    return false;
    /*
    BOOL res;
    HANDLE th = m_Thread->native_handle(); 
    switch (priority) 
    { 
    case TP_REALTIME:   
        res = SetThreadPriority(th, THREAD_PRIORITY_TIME_CRITICAL);   
        break; 
    case TP_HIGHEST: 
        res = SetThreadPriority(th, THREAD_PRIORITY_HIGHEST);                 
        break; 
    case TP_ABOVENORMAL: 
        res = SetThreadPriority(th, THREAD_PRIORITY_ABOVE_NORMAL);    
        break; 
    case TP_NORMAL: 
        res = SetThreadPriority(th, THREAD_PRIORITY_NORMAL);                  
        break; 
    case TP_BACKGROUND: 
        res = SetThreadPriority(th, THREAD_PRIORITY_BELOW_NORMAL);    
        break; 
    case TP_LOWEST: 
        res = SetThreadPriority(th, THREAD_PRIORITY_LOWEST);                  
        break; 
    }
     */

    return false;

}

TaskHandle XCr::TaskThread::SubmitTask( Task* task )
{
    //TaskProcessor::SubmitTask(task);
    m_Runnable->PostTask(task);//Signal();
    return task;
}


void TaskThread::OnThreadExit() {

    ThreadedExecutor::OnThreadExit();
    XLN_OBJ_DELETE m_Runnable;
    m_Runnable = 0;
}

void TaskThread::Stop() {

    //if(m_RunningThread.joinable())
    //    m_RunningThread.join();
    if(m_Runnable)
        m_Runnable->SetFinished();

    ThreadedExecutor::Stop();

}


void TaskThread::OnThreadRun() {

    ThreadedExecutor::OnThreadRun();
    TaskProcessor::ResetCurrent(this);
    m_Runnable->Run();


}


void XCr::TaskThread::WaitTaskComplete(Task* task)
{
    if(m_Runnable)
    {
        m_Runnable->WaitTaskComplete();
    }
}