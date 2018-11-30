#include "Task.h"

#include <XLN/Core/Foundation/Task/TaskProcessor.h>

using namespace XCr;

static std::atomic<int> s_UniqueID(0);

Task::Task(void)
{   
    m_ID = s_UniqueID++;
    //Reset();
    m_CurrentProcessor = TaskProcessor::GetCurrent();
    //m_Category = CATEGORY_ALL;
}

Task::Task(folly::Function<void()> follyFunc)
{
    m_ID = s_UniqueID++;
    //Reset();
    m_CurrentProcessor = TaskProcessor::GetCurrent();
    m_FollyFunc = std::move(follyFunc);
}


Task::~Task(void)
{
    Reset();
}

int XCr::Task::GetID() const
{
    return m_ID;
}

void XCr::Task::Reset()
{
    m_What = 0;
    m_When = 0;
    m_UserParam = 0;
    m_Whom = 0;
    //memset(valParam.m_StrArray, 0, sizeof(ValType));
    m_Category = CATEGORY_ALL;
    m_SysAction = SA_NONE;
    //m_IsCanceled = false;
    m_Message.reset();
    m_State = TS_QUEUED;
    m_FollyFunc = 0;
    m_CurrentProcessor = 0;

    m_Any.clear();

}

void XCr::Task::Cancel()
{
    //m_IsCanceled = true;
}

bool XCr::Task::IsCanceled() const
{
    //return m_IsCanceled;
    return false;
}

void XCr::Task::SetState( TaskState state )
{
    m_State = state;
}

XCr::Task::TaskState XCr::Task::GetState() const
{
    return m_State;
}

void XCr::Task::SetPosion()
{
    m_SysAction = SA_QUIT_RUNNER;
}