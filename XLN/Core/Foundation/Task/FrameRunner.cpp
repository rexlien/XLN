//
// Created by Rex Lien on 4/10/17.
//

#include "FrameRunner.h"
#include <XLN/Core/Foundation/Task/Task.h>
#include <XLN/Core/Foundation/Task/TaskProcessor.h>
#include <XLN/Core/Foundation/Task/TaskHandler.h>
#include <XLN/Core/Foundation/Task/TaskAllocator.h>

using namespace XCr;

FrameRunner::~FrameRunner(void) {

    Task* waitingTask = 0;
    do
    {
        m_WaitingTasks.pop(waitingTask);
        if(waitingTask)
            TaskAllocator::GetInstance().Release(waitingTask);

    }while(waitingTask);
}


void XCr::FrameRunner::DoRun() {

    Task* waitingTask = 0;
    do
    {
        waitingTask = 0;
        if(m_WaitingTasks.pop(waitingTask))
        {
            if(waitingTask->m_SysAction == Task::SA_RELEASE_TASK)
            {
                if(waitingTask->m_Whom)
                {
                    waitingTask->m_Whom->OnFinish(*waitingTask);
                }
                TaskAllocator::GetInstance().Release(waitingTask);
            }
            else
            {
                if(waitingTask->m_SysAction == Task::SA_WAIT_NOTIFIER)
                {
                    //m_WaitCompleteSignal.Signal();
                }
                else
                {
                    if(waitingTask->m_Whom)
                    {
                        waitingTask->m_Whom->Handle(*waitingTask);


                    }
                }
                waitingTask->SetState(Task::TS_HANDLED);

                //if //the task is created in an external thread or is same thread
                if(!waitingTask->m_CurrentProcessor || waitingTask->m_CurrentProcessor == TaskProcessor::GetCurrent())
                {
                    if(waitingTask->m_CurrentProcessor == TaskProcessor::GetCurrent() && waitingTask->m_Whom)
                    {
                        waitingTask->m_Whom->OnFinish(*waitingTask);
                    }
                    TaskAllocator::GetInstance().Release(waitingTask);
                }
                else
                {
                    //if we got thread created the task, send release task job to that thread.
                    if(waitingTask->m_CurrentProcessor)
                    {
                        waitingTask->m_SysAction = Task::SA_RELEASE_TASK;
                        waitingTask->m_CurrentProcessor->SubmitTask(waitingTask);
                    }

                }
            }


        }

    }while(waitingTask);



}


void XCr::FrameRunner::PostTask(XCr::Task *task) {

    m_WaitingTasks.push(task);
}

