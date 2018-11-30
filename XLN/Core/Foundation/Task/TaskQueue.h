#pragma once

#include <XLN/Core/Config/Config.h>
#include <XLN/Core/Utility/Pool.h>

namespace XCr
{

class Task;

class XLN_CLASS_DECL TaskQueue : public XCr::MemObj
{
    public:
        TaskQueue(void);
        ~TaskQueue(void);
        void Enqueue(Task* task);
        Task* Dequeue();
        bool Empty() const;
    
     private:   
        std::priority_queue< Task *> m_Tasks;

};

}