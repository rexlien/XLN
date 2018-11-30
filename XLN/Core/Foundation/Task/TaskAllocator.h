#pragma once

#include <XLN/Core/Config/Config.h>
#include <XLN/Core/Utility/Pool.h>
#include <XLN/Core/Foundation/Task/Task.h>

namespace XCr
{

class XLN_CLASS_DECL TaskAllocator : public FreeList<Task, 2048>
{
    public:
    
        static TaskAllocator& GetInstance();

};

}
//extern TaskAllocator g_TaskAllocator;

