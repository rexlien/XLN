#pragma once

#include <XLN/Core/Config/Config.h>
#include <XLN/Core/Foundation/Task/BlockingQueue.h>
#include <folly/LifoSem.h>
#include <folly/MPMCQueue.h>

namespace XCr
{
    template<class T, QueueBehaviorIfFull kBehavior = QueueBehaviorIfFull::THROW>
    class LifoSemBlockingQueue : public XCr::BlockingQueue<T>
    {
        public:
            static const std::size_t s_DefultCapacity = 1 << 16;
                LifoSemBlockingQueue(std::size_t capacity = s_DefultCapacity)
                : m_Queue(capacity)
            {

            }

            void Add(T item) override
            {
                switch(kBehavior)
                {
                    case QueueBehaviorIfFull::THROW:
                    {
                        if(!m_Queue.write(std::move(item)))
                            throw QueueFullException("Queue full, add item failed");
                    }
                    case QueueBehaviorIfFull::BLOCK:
                        m_Queue.blockingWrite(std::move(item));
                        break;
                }
                m_LifoSem.post();
            }
            
            T Take() override
            {
                T item;
                while(!m_Queue.readIfNotEmpty(item))
                {
                    m_LifoSem.wait();
                }
                return item;
            }

            bool TryTake(T& item)
            {
                return m_Queue.readIfNotEmpty(item);

            }

            std::size_t IsEmpty() const
            {
                return m_Queue.isEmpty();
            }

            std::size_t GetSize() const override
            {
                return m_Queue.size();
            }

        private:
            folly::LifoSem m_LifoSem;
            folly::MPMCQueue<T> m_Queue;
    };
}
