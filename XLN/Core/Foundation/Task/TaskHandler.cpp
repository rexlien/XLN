#include "TaskHandler.h"
#include "Task.h"
#include "TaskThread.h"
#include "TaskProcessor.h"
#include <XLN/Core/Utility/Observer.h>
#include <XLN/Core/Foundation/Task/TaskAllocator.h>
#include <XLN/Core/DebugHelper/Assert.h>
#include <XLN/Core/Utility/Util.h>

using namespace XCr;

TaskProcessor* TaskHandler::ms_MainProcessor = 0;

TaskHandler::TaskHandler(void)
{   
    SetTaskProcessor(ms_MainProcessor);
}

TaskHandler::~TaskHandler(void)
{
}

void XCr::TaskHandler::SetTaskProcessor( TaskProcessor* thread )
{
    m_Processor = thread;
}

TaskHandle XCr::TaskHandler::SendTaskAsync( int what, void* param )
{
    return SendTaskAsync(what, param, this);
}

TaskHandle XCr::TaskHandler::SendTaskAsync( int what, void* param, TaskHandler* target )
{   
    XLN_ASSERT(m_Processor);
    Task* task = TaskAllocator::GetInstance().Aquire();
    task->m_When = XCr::TimeUtil::GetTickCount();
    task->m_What = what;
    task->m_Whom = target;
    m_Processor->SubmitTask(task);
    
    return task;
}

XCr::TaskHandle XCr::TaskHandler::SendTaskAsync( Task::Category category, SharedPtr<IMessage> message, TaskHandler* target )
{
    XLN_ASSERT(m_Processor);
    Task* task = TaskAllocator::GetInstance().Aquire();
    task->m_When = XCr::TimeUtil::GetTickCount();
    task->m_Category = category;
    task->m_Message = message;
    task->m_Whom = target;
    m_Processor->SubmitTask(task);
    return task;

}

void XCr::TaskHandler::Handle( const Task& task )
{
    
    
    
}

void XCr::TaskHandler::CancelTask(TaskHandle taskHandle )
{

}

void XCr::TaskHandler::OnFinish( const Task& userData )
{
    std::list<Observer* >::iterator iter(m_Observers.begin());
    while(iter != m_Observers.end())
    {
        (*iter)->OnEvent(-1, const_cast<Task*>(&userData));
        
        ++iter;
    }
}

void XCr::TaskHandler::OnCancel()
{

}

void XCr::TaskHandler::RegisterObserver( Observer* ob )
{
    m_Observers.push_back(ob);
}

void XCr::TaskHandler::UnRegisterObserver( Observer* ob )
{
    std::list<Observer* >::iterator iter(m_Observers.begin());
    while(iter != m_Observers.end())
    {
        if(*iter == ob)
        {
            m_Observers.erase(iter);
            break;
        }
        ++iter;
    }
    
}

void XCr::TaskHandler::SignalObserver( XCr::UInt32 eventID, void* param )
{
    std::list<Observer* >::iterator iter(m_Observers.begin());
    while(iter != m_Observers.end())
    {
        (*iter)->OnEvent(eventID, param);
        ++iter;
    }
}

void TaskHandler::RegisterMainProcessor( TaskProcessor* mainProcessor )
{
    ms_MainProcessor = mainProcessor;
}