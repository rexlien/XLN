//
// Created by Rex Lien on 08/09/2017.
//
#pragma once

#include <XLN/Core/Config/Config.h>
#include <XLN/Core/Foundation/Task/TaskRunner.h>
#include <XLN/Core/Foundation/Task/BlockingQueue.h>
#include <XLN/Core/Utility/Pool.h>

namespace XCr
{

//template<class TaskAllocator = XCr::FreeList<XCr::Task, 2048>>
class XLN_CLASS_DECL MPMCTaskRunner : public XCr::TaskRunner
{

protected:
    virtual void DoRun() override;


public:
    MPMCTaskRunner(std::unique_ptr<XCr::BlockingQueue<Task *>> blockingQueue);//, TaskAllocator* taskAllocator = 0);
    virtual ~MPMCTaskRunner() = default;
    virtual void PostTask(Task *task) override;

private:

    std::unique_ptr<XCr::BlockingQueue<Task *>> m_TaskQueue;
    //TaskAllocator* m_TaskAllocator;
};


}