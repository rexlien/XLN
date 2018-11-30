//
// Created by Rex Lien on 5/23/17.
//

#include "EventExecutor.h"
#include <XLN/Core/Foundation/Task/TaskAllocator.h>
#include <XLN/Core/Utility/Util.h>

using namespace XCr;

EventExecutor::EventExecutor() {

}

EventExecutor::~EventExecutor() {

}


void EventExecutor::add(folly::Func func) {

    Task* task = TaskAllocator::GetInstance().Aquire();
    task->m_When = XCr::TimeUtil::GetTickCount();
    task->m_FollyFunc = std::move(func);

    SubmitTask(task);
}


void EventExecutor::OnThreadRun() {
    TaskThread::OnThreadRun();
}

