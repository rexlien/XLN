#pragma once

#include <XLN/Core/Config/Config.h>
#include <folly/Executor.h>
#include <XLN/Core/Foundation/Task/MPMCTaskRunner.h>
//#include <XLN/Core/Foundation/ThreadPool.h>
#include <XLN/Core/Foundation/Executor/ThreadPoolExecutor.h>
//#include <C


namespace XCr
{
class XLN_CLASS_DECL CPUThreadPoolExecutor : public ThreadPoolExecutor
{

public:
	
	CPUThreadPoolExecutor();
    virtual ~CPUThreadPoolExecutor();

protected:
	
	class CPUThreadFactory : public XCr::ThreadPool::ThreadFactory
	{
		public:
			CPUThreadFactory(CPUThreadPoolExecutor* executor);
			virtual ThreadPool::Thread* CreateThread() override;

		private:
			CPUThreadPoolExecutor* m_Executor;

	};
	
	class CPUExecutorThread : public XCr::ThreadPoolExecutor::PoolExecutorThread
	{
	public:
		CPUExecutorThread(ThreadPoolExecutor* executor);
		virtual void Exit() override;
	};

    virtual Task *AllocateTask() override;
    virtual void DeallocateTask(Task *task) override;
    virtual void Post(XCr::Task* task);
    virtual void RunThread(ThreadPool::ThreadPtr threadRun) override;
	virtual void AddThread(std::size_t threadToAdd) override;

private:

	//void RunTask(task* task);
    MPMCTaskRunner m_TaskRunner;
	CPUThreadFactory m_ThreadFactory;

};

}