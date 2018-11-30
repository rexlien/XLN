#include "TaskQueue.h"
#include "Task.h"

using namespace XCr;

TaskQueue::TaskQueue(void)
{
}

TaskQueue::~TaskQueue(void)
{
}

void XCr::TaskQueue::Enqueue( Task* task)
{   
    m_Tasks.push(task);
}

Task* XCr::TaskQueue::Dequeue()
{   
    if(m_Tasks.empty())
        return 0;
    
    Task* ret = m_Tasks.top();
    m_Tasks.pop();
   
    return ret;
}

bool XCr::TaskQueue::Empty() const
{
    return m_Tasks.empty();
}