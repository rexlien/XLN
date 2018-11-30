//
// Created by Rex Lien on 08/09/2017.
//

#include "MPMCTaskRunner.h"
#include "LifoSemBlockingQueue.h"
#include "Task.h"
#include "TaskHandler.h"
#include "TaskAllocator.h"
#include "TaskProcessor.h"

using namespace XCr;

XCr::MPMCTaskRunner::MPMCTaskRunner(std::unique_ptr<XCr::BlockingQueue<Task *>> blockingQueue)//,  TaskAllocator* taskAllocator)
    : m_TaskQueue(std::move(blockingQueue))//, m_TaskAllocator(taskAllocator)
{

}



void XCr::MPMCTaskRunner::PostTask(XCr::Task *task)
{
    m_TaskQueue->Add(task);

}


void XCr::MPMCTaskRunner::DoRun()
{
    Task *task = 0;
    while((task = m_TaskQueue->Take()) != 0)
    {
      //t = m_TaskQueue.Take();
      if(task->m_SysAction == Task::SA_QUIT_RUNNER)
      {
          break;
      }
      else
      {
          if(task->m_SysAction == Task::SA_RELEASE_TASK) {
              if(task->m_Whom) {
                  task->m_Whom->OnFinish(*task);
              }
              if(!task->m_DeleteCB) {
                  TaskAllocator::GetInstance().Release(task);
              }
              else
              {
                  task->m_DeleteCB(task);
              }

          }
          else {
              if(task->m_SysAction == Task::SA_WAIT_NOTIFIER) {
                  //m_WaitCompleteSignal.Signal();
              }
			  //actual task
              else {
                  if(task->m_FollyFunc) {
                      task->m_FollyFunc();
                  }
                  else if(task->m_Whom) {
                      task->m_Whom->Handle(*task);

                  }
				  if (m_RunTaskCB)
				  {
					  m_RunTaskCB(task);
				  }
              }
              task->SetState(Task::TS_HANDLED);

              if(!task->m_CurrentProcessor ||
                 task->m_CurrentProcessor == TaskProcessor::GetCurrent()) {
                  if(!task->m_DeleteCB) {
                      TaskAllocator::GetInstance().Release(task);
                  }
                  else
                  {
                      task->m_DeleteCB(task);
                  }
              }
              else {
                  task->m_SysAction = Task::SA_RELEASE_TASK;
                  task->m_CurrentProcessor->SubmitTask(task);
              }
          }
      }

    }
}

