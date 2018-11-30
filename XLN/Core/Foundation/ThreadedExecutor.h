//
// Created by Rex Lien on 3/2/17.
//

#pragma once

#include <XLN/Core/Config/Config.h>
#include <folly/Executor.h>

namespace XCr {

class ThreadedExecutor : public folly::Executor
{
public:
    virtual ~ThreadedExecutor();

    ThreadedExecutor();
    virtual void add(folly::Func func) override;
    virtual void Start();
    virtual void Stop();

protected:

    virtual void OnThreadPreRun();
    virtual void OnThreadRun();
    virtual void OnThreadExit();
    std::thread m_RunningThread;
    std::atomic<bool> m_ThreadExit;

private:
    void ThreadRun();

};

}
