//
// Created by Rex Lien on 4/10/17.
//

#pragma once

#include <XLN/Core/Config/Config.h>
#include <boost/thread/condition.hpp>
#include <boost/thread/mutex.hpp>
#include <XLN/Core/Foundation/Fwd.h>
#include <XLN/Core/Foundation/Task/TaskRunner.h>
//#include <XLN/Core/Platform/Synchronization/WaitableEvent.h>


namespace XCr
{

class TaskQueue;

class XLN_CLASS_DECL TaskConsumer : public TaskRunner
{
public:

    TaskConsumer();
    virtual ~TaskConsumer(void);
    void SetFinished();

    virtual void PostTask(Task* task) override;
    virtual void PostTaskDelay(Task* task, unsigned long time) override;

    void WaitTaskComplete();

    //void OnTick();

protected:

    bool m_Finished;
    virtual void DoRun() override;


private:
    void ReloadWorkQueue();
    void ClearTasks();
    void SignalNewTask();
    std::condition_variable m_NewTaskNotifierCV;
    std::mutex m_NewTaskNotifierMutex;
    bool m_NewTaskReady;


    //WaitableEvent m_TaskNotifierr;
    //WaitableEvent m_WaitCompleteSignal;
    //bool m_Waiting;

    //working queues
    boost::mutex m_WorkQueueReloadMutex;
    XCr::TaskQueue* m_IncomingQueue;
    XCr::TaskQueue* m_WorkingQueue;

    //finished queue
    boost::mutex m_WorkFinishedQueueReloadMutex;
    //XCr::TaskQueue* m_IncomingFinishedTaskQueue;
    //XCr::TaskQueue* m_WorkingFinishedTaskQueue;

};

}