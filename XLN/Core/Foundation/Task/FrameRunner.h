//
// Created by Rex Lien on 4/10/17.
//

#pragma once
#include <XLN/Core/Config/Config.h>
#include <XLN/Core/Foundation/Task/TaskRunner.h>
#include <boost/lockfree/queue.hpp>


namespace XCr {

class Task;

class XLN_CLASS_DECL FrameRunner : public TaskRunner
{
public:
    virtual ~FrameRunner(void) override;
    virtual void PostTask(Task *task) override;
protected:
    virtual void DoRun() override;

private:
    boost::lockfree::queue<Task *, boost::lockfree::capacity<2048> > m_WaitingTasks;



};

}
