//
// Created by Rex Lien on 4/10/17.
//

#include "TaskConsumer.h"
#include <XLN/Core/Foundation/Log.h>
#include <XLN/Core/Foundation/Task/Task.h>
#include "TaskHandler.h"
#include <XLN/Core/Foundation/Task/TaskAllocator.h>
#include <XLN/Core/Foundation/Task/TaskProcessor.h>
#include <XLN/Core/Foundation/Task/TaskQueue.h>
#include <XLN/Core/Utility/Util.h>

using namespace XCr;

TaskConsumer::TaskConsumer()
{
    m_Finished = false;
    m_NewTaskReady = false;
    //m_TaskQueue = queue;
    m_IncomingQueue = XLN_OBJ_NEW XCr::TaskQueue();
    m_WorkingQueue = XLN_OBJ_NEW XCr::TaskQueue();

    //m_IncomingFinishedTaskQueue = XLN_OBJ_NEW XCr::TaskQueue();
    //m_WorkingFinishedTaskQueue = XLN_OBJ_NEW XCr::TaskQueue();

}


TaskConsumer::~TaskConsumer(void)
{
    XLN_OBJ_DELETE m_IncomingQueue;
    XLN_OBJ_DELETE m_WorkingQueue;
    //XLN_OBJ_DELETE m_IncomingFinishedTaskQueue;
    //XLN_OBJ_DELETE m_WorkingFinishedTaskQueue;

}


void XCr::TaskConsumer::DoRun()
{
    while(!m_Finished)
    {

        Task* task = 0;

        {
            std::unique_lock<std::mutex> lock(m_NewTaskNotifierMutex);
            while(!m_NewTaskReady)
            {
                m_NewTaskNotifierCV.wait(lock);
            }
            ReloadWorkQueue();
            m_NewTaskReady = false;
        }

        do
        {
            if(m_Finished)
                break;

            task = m_WorkingQueue->Dequeue();
            if(task)
            {
                if(task->m_SysAction == Task::SA_RELEASE_TASK)
                {
                    if(task->m_Whom)
                    {
                        task->m_Whom->OnFinish(*task);
                    }
                    TaskAllocator::GetInstance().Release(task);

                }
                else
                {
                    if(task->m_SysAction == Task::SA_WAIT_NOTIFIER)
                    {
                        //m_WaitCompleteSignal.Signal();
                    }
                    else
                    {
                        if(task->m_FollyFunc)
                        {
                            task->m_FollyFunc();
                        }
                        else if(task->m_Whom)
                        {
                            task->m_Whom->Handle(*task);


                        }
                    }
                    task->SetState(Task::TS_HANDLED);

                    if(!task->m_CurrentProcessor || task->m_CurrentProcessor == TaskProcessor::GetCurrent())
                    {
                        TaskAllocator::GetInstance().Release(task);
                    }
                    else
                    {
                        task->m_SysAction = Task::SA_RELEASE_TASK;
                        task->m_CurrentProcessor->SubmitTask(task);
                    }
                }



                //TaskAllocator::GetInstance().Release(task);
            }

        } while(task);

        //m_TaskWaiter.Wait();

    }

    ClearTasks();
}

void XCr::TaskConsumer::SignalNewTask()
{


}

void XCr::TaskConsumer::SetFinished()
{

    {
        std::lock_guard<std::mutex> lock(m_NewTaskNotifierMutex);
        m_Finished = true;
        m_NewTaskReady = true;
    }
    m_NewTaskNotifierCV.notify_all();

}

void XCr::TaskConsumer::ReloadWorkQueue()
{

    //boost::unique_lock<boost::mutex> lock(m_WorkQueueReloadMutex);
    if(m_IncomingQueue->Empty())
    {
        return;
    }
    std::swap(m_IncomingQueue, m_WorkingQueue);
    //lock.unlock();

}

void XCr::TaskConsumer::PostTask( Task* task )
{

    {
        std::lock_guard<std::mutex> lock(m_NewTaskNotifierMutex);
        m_IncomingQueue->Enqueue(task);
        m_NewTaskReady = true;
    }
    m_NewTaskNotifierCV.notify_all();


}

void XCr::TaskConsumer::PostTaskDelay( Task* task, unsigned long time )
{

    {
        std::lock_guard<std::mutex> lock(m_NewTaskNotifierMutex);
        m_IncomingQueue->Enqueue(task);
        m_NewTaskReady = true;
    }
    m_NewTaskNotifierCV.notify_all();

}

//NOTE: should only called in internal thread
void XCr::TaskConsumer::ClearTasks()
{

    Task* task = m_WorkingQueue->Dequeue();
    while(task)
    {
        TaskAllocator::GetInstance().Release(task);
        task = m_WorkingQueue->Dequeue();
    }

    task = m_IncomingQueue->Dequeue();
    while(task)
    {
        TaskAllocator::GetInstance().Release(task);
        task = m_IncomingQueue->Dequeue();
    }

    {
        //boost::unique_lock<boost::mutex> lock(m_WorkQueueReloadMutex);
        //task = m_IncomingFinishedTaskQueue->Dequeue();
        //while(task)
        //{
        //    TaskAllocator::GetInstance().Release(task);
        //    if(task->m_Whom)
        //        task->m_Whom->OnCancel();
        //    task = m_IncomingFinishedTaskQueue->Dequeue();
        //}
    }

}


void XCr::TaskConsumer::WaitTaskComplete()
{
    Task* task = TaskAllocator::GetInstance().Aquire();
    task->m_When = XCr::TimeUtil::GetTickCount();
    task->m_What = -1;
    task->m_Whom = 0;
    task->m_SysAction = Task::SA_WAIT_NOTIFIER;
    PostTask(task);

    //m_TaskWaiter.Signal();
    //m_WaitCompleteSignal.Wait();

}