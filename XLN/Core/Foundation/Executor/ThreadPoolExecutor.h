//
// Created by Rex Lien on 18/09/2017.
//

#pragma once

#include <XLN/Core/Config/Config.h>
#include <folly/Executor.h>
#include <XLN/Core/Foundation/ThreadPool.h>
#include <XLN/Core/Foundation/Fwd.h>
#include <XLN/Core/Utility/Pool.h>
#include <XLN/Core/Foundation/Task/Task.h>
#include <XLN/Core/Config/Memory.h>


namespace XCr
{
//template<class TaskAllocator = XCr::FreeList<XCr::Task, 2048>>
class XLN_CLASS_DECL ThreadPoolExecutor : public folly::Executor, public XCr::MemObj
{

public:
    class ThreadPoolTask : public Task
    {
    public:
        bool posion;

    };
	
	class PoolExecutorThread : public XCr::ThreadPool::Thread
	{
		public:
			PoolExecutorThread(ThreadPoolExecutor* executor);


		protected:
			ThreadPoolExecutor* m_Executor;
	};

    //class

    ThreadPoolExecutor();
    virtual ~ThreadPoolExecutor();

    virtual void add(folly::Func func);
    virtual void Post(XCr::Task* task) = 0;
    void SetMaxThread(std::size_t numOfThread);
    void StopAllThread();
	
protected:
	virtual void RunTask(Task* task);
    virtual void RunThread(ThreadPool::ThreadPtr threadRun) = 0;
	
	
    virtual void AddThread(std::size_t threadToAdd);
    virtual void RemoveThread(std::size_t threadToRemove);

    virtual Task* AllocateTask() = 0;
    virtual void DeallocateTask(Task* task) = 0;

    ThreadPool m_ThreadPool;
    std::size_t m_MaxThread;
    std::size_t  m_CurThreadCount;

    //XCr::SharedPtr<TaskAllocator> m_TaskAllocator;


};


}