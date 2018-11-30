#include <XLN/Core/Foundation/Task/TaskAllocator.h>

using namespace XCr;

TaskAllocator& TaskAllocator::GetInstance()
{
    static TaskAllocator inst;
    return inst;   
}