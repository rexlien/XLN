#pragma once

#include <XLN/Core/Config/Config.h>
#include <XLN/Core/Utility/SharedPtr.h>

namespace XCr
{

class Task;

class XLN_CLASS_DECL Observer : public XCr::MemObj
{
    public:
        Observer(void);
        virtual ~Observer(void);
        
        virtual void OnEvent(UInt32 eventID, void* param) = 0;
};

XLN_SHARED_PTR(Observer);
}

