#pragma once

#include <boost/thread/thread.hpp>
#include <XLN/Core/Foundation/ThreadedExecutor.h>
#include "TaskProcessor.h"


//#include "Taskrunner.h"

namespace XCr
{
class TaskRunner;
class TaskQueue;
class Task;

class XLN_CLASS_DECL TaskThread : public TaskProcessor, public XCr::ThreadedExecutor
{
public:
    enum ThreadPriotiy
    {   
        TP_REALTIME,
        TP_HIGHEST,
        TP_ABOVENORMAL,
        TP_NORMAL,
        TP_BACKGROUND,
        TP_LOWEST

    };

    virtual void Stop() override;

    TaskThread();
    TaskThread(TaskRunner* runner);
    ~TaskThread();
    bool SetPriority(ThreadPriotiy priority);
    virtual TaskHandle SubmitTask(Task* task);
    virtual void WaitTaskComplete(Task* task);

protected:
    virtual void OnThreadExit() override;
    virtual void OnThreadRun() override;






    
private:
    //boost::thread* m_Thread;
    //XCr::WaitableEvent m_StartupWaitEvent;
    XCr::TaskRunner* m_Runnable;
    
    


};

}
