#include "EventHandler.h"
#include "EventDispatcher.h"
#include "Task.h"

#include <XLN/Core/Foundation/Task/TaskAllocator.h>

using namespace XCr;

XCr::EventHandler::EventHandler()
{
    m_EventHandlerMgr = 0;
}   

EventHandler::~EventHandler(void)
{
    //if(m_EventHandlerMgr)
    //    m_EventHandlerMgr->DeSubscribeEvent(this);
}

void XCr::EventHandler::SubscribeEvent()
{
    m_EventHandlerMgr->SubscribeEvent(this);
}

void XCr::EventHandler::SubscribeEvent( EventDispatcher* eventHandlerMgr )
{
    SetEventDispatcher(eventHandlerMgr);
    m_EventHandlerMgr->SubscribeEvent(this);
}

void XCr::EventHandler::DeSubscribeEvent( )
{
    m_EventHandlerMgr->DeSubscribeEvent(this);
}

void XCr::EventHandler::BroadcastTask( XCr::Int32 what, void* param, Task::Category category, SharedPtr<IMessage> message)
{   
    Task* task = TaskAllocator::GetInstance().Aquire();
    task->m_What = what;
    task->m_UserParam = param;
    task->m_Category = category;
    task->m_Message = message;
    m_EventHandlerMgr->Handle(*task);
    TaskAllocator::GetInstance().Release(task);
}

void XCr::EventHandler::BroadcastTaskAsync( int what, void* param )
{
    m_EventHandlerMgr->SendTaskAsync(what, param);
}

void XCr::EventHandler::SetEventDispatcher( EventDispatcher* eventMgr )
{
    m_EventHandlerMgr = eventMgr;
}

XCr::Task::Category XCr::EventHandler::GetCategory() const
{
    return Task::CATEGORY_SYSTEM;
}

void XCr::EventHandler::Handle( const Task& task )
{   
    if(task.m_Message)
        OnHandleMessage(task.m_Message);
}

const XCr::EventHandler::EventFilterList* XCr::EventHandler::GetEventFilterClassID() const
{
    return 0;
}