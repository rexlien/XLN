//
// Created by Rex Lien on 07/09/2017.
//

#include "ThreadPool.h"

using namespace XCr;

std::atomic<std::uint64_t> ThreadPool::Thread::m_NextUID(0);

XCr::ThreadPool::ThreadPool()
{
	m_CurrentPickThread = 0;
}

void ThreadPool::AddThread(std::size_t numToAdd, std::function<void(ThreadPtr)>&& runFunc)
{
    while(numToAdd)
    {
        auto newThread = std::make_shared<Thread>();	
		newThread->SetThread(std::thread(std::bind(runFunc, newThread)));
		{
			folly::RWSpinLock::WriteHolder w(&m_ThreadListLock);
			m_Threads.Add(newThread);
		}
        numToAdd--;
    }

}

//template<class T = ThreadFactory>

void XCr::ThreadPool::AddThread(std::size_t numToAdd, std::function<void(ThreadPtr)>&& runFunc, ThreadFactory * threadfactory)
{
	while (numToAdd)
	{
		auto newThread = XCr::SharedPtr<Thread>(threadfactory->CreateThread());// ThreadFactory::CreateThread());
		newThread->SetThread(std::thread(std::bind(runFunc, newThread)));
		
		{	
			folly::RWSpinLock::WriteHolder w(&m_ThreadListLock);
			m_Threads.Add(newThread);
		}
		numToAdd--;
	}
}

XCr::ThreadPool::ThreadPtr XCr::ThreadPool::PickThread()
{
	folly::RWSpinLock::ReadHolder r(&m_ThreadListLock);
	std::size_t count = GetThreadCount();
	if (count == 0)
		return 0;
	
	std::size_t pickCount = m_CurrentPickThread++ % count;
	return m_Threads.At(pickCount);

}

void XCr::ThreadPool::ForAllThread(folly::Function<void(ThreadPtr)> func)
{
	folly::RWSpinLock::ReadHolder r(&m_ThreadListLock);
	m_Threads.Foreach(std::move(func));
	
}

void ThreadPool::RemoveThread(std::size_t numToRemove)
{
	folly::RWSpinLock::ReadHolder r(&m_ThreadListLock);
	std::size_t actualRemove = std::min(m_Threads.Size(), numToRemove);
	for (std::size_t i = 0; i < actualRemove; i++)
	{
		auto thread = m_Threads.At(i);
		thread->Exit();
	}
}


void ThreadPool::RemoveThread(ThreadPtr threadToRemove)
{
	folly::RWSpinLock::WriteHolder w(&m_ThreadListLock);
    m_Threads.Remove(threadToRemove);
}


std::size_t ThreadPool::GetThreadCount() const {

    folly::RWSpinLock::ReadHolder r(&m_ThreadListLock);
    return m_Threads.Size();

}


void ThreadPool::StopAllThread()
{
    std::size_t threadCount = GetThreadCount();
    JoinStoppedThread(threadCount);
}



void ThreadPool::AddStoppingThread(ThreadPool::ThreadPtr stoppingThread)
{
    //folly::RWSpinLock::WriteHolder  w(m_ThreadListLock);
    m_StoppedThread.Add(stoppingThread);
}


void ThreadPool::JoinStoppedThread(std::size_t numToJoin)
{
    for(std::size_t i = 0; i < numToJoin; i++)
    {
        auto thread = m_StoppedThread.Take();
        if(thread->m_Thread.joinable())
        {
            thread->m_Thread.join();
        }
		RemoveThread(thread);
    }
}


