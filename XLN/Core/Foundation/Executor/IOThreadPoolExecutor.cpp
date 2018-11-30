//
// Created by Rex Lien on 18/09/2017.
//

#include "IOThreadPoolExecutor.h"
#include <folly/io/async/EventBaseManager.h>
#include <XLN/Core/Foundation/Task/TaskAllocator.h>

using namespace XCr;

IOThreadPoolExecutor::IOThreadPoolExecutor()
    :ThreadPoolExecutor()
{

}

IOThreadPoolExecutor::~IOThreadPoolExecutor() {

}


void IOThreadPoolExecutor::RunThread(ThreadPool::ThreadPtr threadRun) 
{
	auto ioThread = std::static_pointer_cast<IOThreadPoolExecutor::IOThread>(threadRun);
	m_ThisThread.reset(new ThreadPool::ThreadPtr(threadRun));
	ioThread->m_EventBase = folly::EventBaseManager::get()->getEventBase();
	ioThread->m_ThreadStartBarrier.post();

	ioThread->m_EventBase->loopForever();
	m_ThreadPool.AddStoppingThread(threadRun);

	
}

void IOThreadPoolExecutor::AddThread(std::size_t threadToAdd)
{
	m_ThreadPool.AddThread(threadToAdd, std::bind(&IOThreadPoolExecutor::RunThread, this, std::placeholders::_1), &m_IOThreadFactory);
}


void IOThreadPoolExecutor::add(folly::Func func) {

	auto ioThread = std::static_pointer_cast<IOThreadPoolExecutor::IOThread>(*m_ThisThread);
	if (!ioThread)
	{
		ioThread = std::static_pointer_cast<IOThreadPoolExecutor::IOThread>(m_ThreadPool.PickThread());
	}
	if (ioThread)
	{
		ioThread->m_EventBase->runInEventBaseThread(std::move(func));
	}

}

void XCr::IOThreadPoolExecutor::addAll(std::function<void()> func)
{
	auto lambda = [func](ThreadPool::ThreadPtr thread) mutable
	{
		auto ioThread = std::static_pointer_cast<IOThreadPoolExecutor::IOThread>(thread);
        
        ioThread->m_EventBase->runInEventBaseThread(func);
		
	};
	m_ThreadPool.ForAllThread(std::move(lambda));
}

XCr::SharedPtr<IOThreadPoolExecutor::IOThread> XCr::IOThreadPoolExecutor::GetCurrentIOThread()
{
	return std::static_pointer_cast<IOThreadPoolExecutor::IOThread>(*m_ThisThread);
}

void IOThreadPoolExecutor::Post(XCr::Task *task) {

}

Task *IOThreadPoolExecutor::AllocateTask() {
    return TaskAllocator::GetInstance().Aquire();
}

void IOThreadPoolExecutor::DeallocateTask(Task *task) {
	return TaskAllocator::GetInstance().Release(task);
}



XCr::ThreadPool::Thread* XCr::IOThreadPoolExecutor::IOThreadFactory::CreateThread()
{
	return XLN_OBJ_NEW IOThread();
}

void XCr::IOThreadPoolExecutor::IOThread::SetThread(std::thread&& thread)
{
	ThreadPool::Thread::SetThread(std::move(thread));
	this->m_ThreadStartBarrier.wait();
}

void XCr::IOThreadPoolExecutor::IOThread::Exit()
{
	m_EventBase->terminateLoopSoon();
}
