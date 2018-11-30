#pragma once

#include <XLN/Core/Config/Config.h>
#include <XLN/Core/Foundation/Task/BlockingQueue.h>
#include <folly/LifoSem.h>
#include <mutex>
//#include <thread>

namespace XCr
{
    template<class T>
    class XLN_CLASS_DECL TaskBarrierBlockingQueue : public XCr::BlockingQueue<T>
    {
        public:
            virtual void Add(T task) override
            {
                {
                    std::lock_guard<std::mutex> scopeLock(m_TaskMutex);
                    m_Tasks.push(task);
                }
                m_Sem.post();

            }
            virtual T Take() override
            {
                while(true)
                {
                    {
                        std::lock_guard<std::mutex> scopeLock(m_TaskMutex);
                        if(!m_Tasks.empty())
                        {
                            auto task = m_Tasks.front();
                            m_Tasks.pop();
                            return task;

                        }
                    }

                    m_Sem.wait();
                }
            }
            virtual std::size_t GetSize() const override
            {
                {
                    std::lock_guard<std::mutex> scopeLock(m_TaskMutex);
                    return m_Tasks.size();
                }
            }

        private:
            folly::LifoSem m_Sem;
            mutable std::mutex m_TaskMutex;
            std::queue<T> m_Tasks;

    };
}