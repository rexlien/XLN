#include "TaskProcessor.h"

#include <stdlib.h>

#include "TaskQueue.h"
#include "TaskHandler.h"
#include "Task.h"
#include <XLN/Core/Foundation/Task/TaskAllocator.h>
#include <boost/thread/lock_guard.hpp>


using namespace XCr;


static void NullDestroyer(TaskProcessor*)
{
    return;
}

boost::thread_specific_ptr<TaskProcessor> TaskProcessor::m_Current(NullDestroyer);

TaskProcessor::TaskProcessor(void)
{   
    m_IncomingQueue = XLN_OBJ_NEW XCr::TaskQueue();
    m_WorkingQueue = XLN_OBJ_NEW XCr::TaskQueue();
}

TaskProcessor::~TaskProcessor(void)
{
    Free(); 
}

TaskHandle XCr::TaskProcessor::SubmitTask( Task* task )
{
    boost::lock_guard<boost::mutex> lock(m_WorkQueueReloadMutex);
    m_IncomingQueue->Enqueue(task); 
    return task;
}
/*
void XCr::TaskProcessor::Execute()
{   
    boost::unique_lock<boost::mutex> lock(m_WorkQueueReloadMutex);   
    if(!m_IncomingQueue->Empty())
        std::swap(m_IncomingQueue, m_WorkingQueue);
    lock.unlock();
    
    Task* task = 0;
    do
    {   task = m_WorkingQueue->Dequeue();
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
            {   if(task->m_Whom)
                    task->m_Whom->Handle(*task);
                
                task->SetState(Task::TS_HANDLED);
                if(task->m_CurrentProcessor == TaskProcessor::GetCurrent())
                {
                    TaskAllocator::GetInstance().Release(task);
                }
                else
                {
                    task->m_SysAction = Task::SA_RELEASE_TASK;
                    task->m_CurrentProcessor->SubmitTask(task);
                }
                
            }
        }

    } while(task);
}
*/


void XCr::TaskProcessor::Free()
{   
    CleanTasks();
    if(m_IncomingQueue)
    {   
        XLN_OBJ_DELETE m_IncomingQueue;
        m_IncomingQueue = 0;
    }
    if(m_WorkingQueue)
    {   
        XLN_OBJ_DELETE m_WorkingQueue;
        m_WorkingQueue = 0;
    }
}   

/*
void XCr::TaskProcessor::CancelTask( TaskHandle task )
{

}
*/
TaskProcessor* XCr::TaskProcessor::GetCurrent()
{
    return m_Current.get();
    
}

void XCr::TaskProcessor::ResetCurrent( TaskProcessor* current )
{   
    m_Current.reset(current);
}

void XCr::TaskProcessor::Init()
{

}

void XCr::TaskProcessor::CleanTasks()
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

}

int XCr::GetTaskID( TaskHandle handle )
{
    return (((Task*)handle)->GetID());    
}