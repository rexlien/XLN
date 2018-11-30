//
// Created by Rex Lien on 18/09/2017.
//

#include "ThreadPoolExecutor.h"
#include <XLN/Core/Foundation/Task/TaskAllocator.h>
#include <folly/Singleton.h>


using namespace XCr;

static folly::Singleton<ThreadPool::ThreadFactory> s_DefFactory;

XCr::ThreadPoolExecutor::ThreadPoolExecutor()
{
    m_MaxThread = 0;
    m_CurThreadCount = 0;


}

ThreadPoolExecutor::~ThreadPoolExecutor()
{
    //StopAllThread();
}

void ThreadPoolExecutor::StopAllThread()
{	
	m_ThreadPool.RemoveThread(m_CurThreadCount);
    m_ThreadPool.JoinStoppedThread(m_CurThreadCount);
    m_CurThreadCount = m_ThreadPool.GetThreadCount();

}

void ThreadPoolExecutor::SetMaxThread(std::size_t numOfThread)
{
    if(m_CurThreadCount != numOfThread)
    {   if(m_CurThreadCount > numOfThread)
        {
            RemoveThread(m_CurThreadCount - numOfThread);
        }
        else
        {
            AddThread(numOfThread - m_CurThreadCount);
        }
        m_MaxThread = numOfThread;
        m_CurThreadCount = m_ThreadPool.GetThreadCount();
    }
}

void ThreadPoolExecutor::RemoveThread(std::size_t threadToRemove)
{
	m_ThreadPool.RemoveThread(threadToRemove);
    m_ThreadPool.JoinStoppedThread(threadToRemove);
    m_CurThreadCount = m_ThreadPool.GetThreadCount();
}

void ThreadPoolExecutor::add(folly::Func func)
{
    XCr::Task* task = AllocateTask();
    task->m_DeleteCB = std::bind(&ThreadPoolExecutor::DeallocateTask, this, std::placeholders::_1);
    task->m_FollyFunc = std::move(func);
    Post(task);
    //m_TaskRunner.PostTask(task);
}


void XCr::ThreadPoolExecutor::RunTask(Task * task)
{
}

void ThreadPoolExecutor::AddThread(std::size_t threadToAdd)
{

    m_ThreadPool.AddThread(threadToAdd, std::bind(&ThreadPoolExecutor::RunThread, 
			this, std::placeholders::_1), folly::Singleton<ThreadPool::ThreadFactory>::try_get().get());
}

XCr::ThreadPoolExecutor::PoolExecutorThread::PoolExecutorThread(ThreadPoolExecutor* executor) :m_Executor(executor)
{

}
