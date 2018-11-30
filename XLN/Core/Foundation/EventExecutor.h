
#pragma once

#include <XLN/Core/Config/Config.h>
#include <XLN/Core/Foundation/Task/TaskThread.h>

namespace XCr
{

class XLN_CLASS_DECL EventExecutor : public XCr::TaskThread
{

public:
    EventExecutor();
    ~EventExecutor();
protected:
    virtual void OnThreadRun() override;

    virtual void add(folly::Func func) override;
};

}