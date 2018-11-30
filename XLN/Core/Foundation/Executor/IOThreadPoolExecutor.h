//
// Created by Rex Lien on 18/09/2017.
//
#pragma once

#include <XLN/Core/Config/Config.h>
#include <XLN/Core/Foundation/Executor/ThreadPoolExecutor.h>
#include <folly/io/async/EventBase.h>
#include <folly/ThreadLocal.h>

namespace XCr
{

class XLN_CLASS_DECL IOThreadPoolExecutor : public XCr::ThreadPoolExecutor
{

public:
	
	class IOThread : public ThreadPool::Thread
	{
		public:
			folly::EventBase* m_EventBase;
			virtual void SetThread(std::thread&& thread) override;
			virtual void Exit() override;

	};

	class IOThreadFactory : public ThreadPool::ThreadFactory
	{
	public:
		virtual ThreadPool::Thread* CreateThread() override;

	};

    virtual void add(folly::Func func) override;
	virtual void addAll(std::function<void()> func);
    virtual void Post(XCr::Task *task) override;
	XCr::SharedPtr<IOThreadPoolExecutor::IOThread> GetCurrentIOThread();

    IOThreadPoolExecutor();
    virtual ~IOThreadPoolExecutor();

protected:
    virtual Task *AllocateTask() override;
    virtual void DeallocateTask(Task *task) override;
    virtual void RunThread(ThreadPool::ThreadPtr threadRun) override;
	
	virtual void AddThread(std::size_t threadToAdd) override;
	

private:
	IOThreadFactory m_IOThreadFactory;
	folly::ThreadLocal<ThreadPool::ThreadPtr> m_ThisThread;


};

}