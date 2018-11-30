//
// Created by Rex Lien on 07/09/2017.
//

#pragma once

#include <XLN/Core/Config/Config.h>
#include <XLN/Core/Utility/SharedPtr.h>
#include <XLN/Core/Foundation/Task/TaskBarrierBlockingQueue.h>
#include <folly/RWSpinLock.h>
#include <folly/concurrency/CacheLocality.h>
#include <folly/Function.h>
#include <chrono>
#include <algorithm>

namespace XCr
{
class XLN_CLASS_DECL ThreadPool
{
public:
	

    class Thread : public XCr::MemObj
    {
	public:
        Thread()
        : m_ID(m_NextUID++) {
            m_Idle = true;
        }

        virtual ~Thread() = default;
		virtual void Exit() {};
		virtual void SetThread(std::thread&& thread) { m_Thread = std::move(thread); }

		//void SetName(const std::string& name);
        //private:
        std::thread m_Thread;
        std::uint64_t m_ID;
        std::chrono::steady_clock::time_point m_LastActiveTime;
		folly::Baton<> m_ThreadStartBarrier;
		
        bool m_Idle;
	private:
		std::string m_Name;
        static std::atomic<std::uint64_t> m_NextUID;

    };

	class ThreadFactory
	{
	public:
		virtual ~ThreadFactory()
		{

		}

		virtual Thread* CreateThread()
		{
			return XLN_OBJ_NEW Thread();
		}

	};
	

    //class XLN_CLASS_DECL ThreadObserver

    typedef XCr::SharedPtr<Thread> ThreadPtr;
    class ThreadList {
    public:
        void Add(const ThreadPtr& state) {
            auto it = std::lower_bound(m_Threads.begin(), m_Threads.end(), state,
            // compare method is a static method of class
            // and therefore cannot be inlined by compiler
            // as a template predicate of the STL algorithm
            // but wrapped up with the lambda function (lambda will be inlined)
            // compiler can inline compare method as well
                                       [&](const ThreadPtr& ts1, const ThreadPtr& ts2) -> bool { // inline
                                           return compare(ts1, ts2);
                                       });
            m_Threads.insert(it, state);
        }

        void Remove(const ThreadPtr& state) {
            auto itPair = std::equal_range(m_Threads.begin(), m_Threads.end(), state,
            // the same as above
                                           [&](const ThreadPtr& ts1, const ThreadPtr& ts2) -> bool { // inline
                                               return compare(ts1, ts2);
                                           });
            CHECK(itPair.first != m_Threads.end());
            CHECK(std::next(itPair.first) == itPair.second);
            m_Threads.erase(itPair.first);
        }

        const std::vector<ThreadPtr>& Get() const {
            return m_Threads;
        }

        std::size_t Size() const {
            return m_Threads.size();
        }

		ThreadPtr At(std::size_t at)
		{
			return m_Threads[at];
		}

		void Foreach(folly::Function<void(ThreadPtr)> func)
		{
			for (auto thread : m_Threads)
			{
				func(thread);
			}
		}

    private:
        static bool compare(const ThreadPtr& ts1, const ThreadPtr& ts2) {
            return ts1->m_ID < ts2->m_ID;
        }

        std::vector<ThreadPtr> m_Threads;
    };


    ThreadPool();

    void SetNumOfThread(XCr::UInt32 num);

    void AddThread(std::size_t numToAdd, std::function<void(ThreadPtr)>&& runFunc);
	void AddThread(std::size_t numToAdd, std::function<void(ThreadPtr)>&& runFunc, ThreadFactory* threadfactory);

    //
    void RemoveThread(std::size_t numToRemove);
    void StopAllThread();
    void AddStoppingThread(ThreadPtr stoppingThread);
    void JoinStoppedThread(std::size_t numToJoin);
    std::size_t GetThreadCount() const;
	ThreadPtr PickThread();
	void ForAllThread(folly::Function<void(ThreadPtr)> func);

private:
	void RemoveThread(ThreadPtr threadToRemove);
    ThreadList m_Threads;
    XCr::TaskBarrierBlockingQueue<ThreadPool::ThreadPtr> m_StoppedThread;
    mutable folly::RWSpinLock m_ThreadListLock;
	std::size_t m_CurrentPickThread;
	//folly::Singleton<ThreadFactory> m_DefaultThreadFactory;
};

}
