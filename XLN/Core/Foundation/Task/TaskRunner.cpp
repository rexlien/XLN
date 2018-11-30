#include "TaskRunner.h"
#include <XLN/Core/Foundation/Log.h>
//#include "tbb/concurrent_queue.h"

#include "TaskQueue.h"
#include <XLN/Core/Foundation/Task/Task.h>
#include "TaskHandler.h"
#include <XLN/Core/Foundation/Task/TaskAllocator.h>
#include <XLN/Core/Foundation/Task/TaskProcessor.h>
#include <XLN/Core/Utility/Util.h>

using namespace XCr;

TaskRunner::TaskRunner()
{

    
}   


TaskRunner::~TaskRunner(void)
{

    
}

void XCr::TaskRunner::Run()
{
    
    DoRun();
}





void XCr::TaskRunner::PostTaskDelay( Task* task, unsigned long time )
{



}

void TaskRunner::SetFinished() {

}


void XCr::TaskRunner::WaitTaskComplete()
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

void XCr::TaskRunner::SetRunTaskCB(TaskRunCB cb)
{
	m_RunTaskCB = cb;
}
