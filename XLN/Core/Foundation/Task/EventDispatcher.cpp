#include "EventDispatcher.h"

#include <XLN/Core/Foundation/Task/Task.h>
#include <XLN/Core/Foundation/Task/EventHandler.h>
//#include <XLN/Core/Foundation/Profiler.h>

using namespace XCr;

EventDispatcher::EventDispatcher(void)
{   
    m_EventSubscribers.reserve(Task::CATEGORY_MAX);
    m_EventSubscribers.resize(Task::CATEGORY_MAX);
}

EventDispatcher::~EventDispatcher(void)
{

}

void XCr::EventDispatcher::SubscribeEvent( EventHandler* handler )
{
    const EventHandler::EventFilterList* filterList = handler->GetEventFilterClassID();
    if(filterList)
    {   
        EventHandler::EventFilterList::const_iterator iter(filterList->begin());
        while(iter != filterList->end())
        {   m_EventHandlers.insert(EventHandlerHash::value_type((*iter), handler));
            ++iter;
        }
    }
    else
    {   std::list<EventHandler*>& targetList = m_EventSubscribers[handler->GetCategory()];
        targetList.push_back(handler);
    }
}

void XCr::EventDispatcher::DeSubscribeEvent( EventHandler* handler )
{   
    const EventHandler::EventFilterList* filterList = handler->GetEventFilterClassID();
     if(filterList)
    {   
        EventHandler::EventFilterList::const_iterator iter(filterList->begin());
        while(iter != filterList->end())
        {   std::pair<EventHandlerHash::iterator, EventHandlerHash::iterator> range = m_EventHandlers.equal_range((*iter));
            for (; range.first != range.second; ++range.first) 
            {
                if(range.first->second == handler)
                {
                    m_EventHandlers.erase(range.first);
                    break;
                }
            }
            ++iter;
        }
    }
    else
    {   std::list<EventHandler*>& targetList = m_EventSubscribers[handler->GetCategory()];
        targetList.remove(handler);
    }
}

void XCr::EventDispatcher::Handle( const Task& task )
{
    //XLN_PROFILE("EventDispatcher::Handle()");
    std::pair<EventHandlerHash::iterator, EventHandlerHash::iterator> range = m_EventHandlers.equal_range(task.m_Message->ObjClassID());

    for (auto it = range.first; it != range.second; ++it)
    {
        it->second->Handle(task);
    }
    
    if(task.m_Category != Task::CATEGORY_ALL)           
    {   std::list<EventHandler*>& targetList = m_EventSubscribers[task.m_Category];
        std::list<EventHandler*>::iterator iter(targetList.begin());
        while(iter != targetList.end())
        {   (*iter)->Handle(task);
            ++iter;
        }
    }
    else
    {
        for(XCr::UInt32 i = 0; i < Task::CATEGORY_MAX; i++)
        {
            std::list<EventHandler*>& targetList = m_EventSubscribers[i];
            std::list<EventHandler*>::iterator iter(targetList.begin());
            while(iter != targetList.end())
            {   (*iter)->Handle(task);
                ++iter;
            }
        }
    }    
  
}

void XCr::EventDispatcher::OnFinish( const Task& task )
{   
    std::pair<EventHandlerHash::iterator, EventHandlerHash::iterator> range = m_EventHandlers.equal_range(task.m_Message->ObjClassID());
    for (; range.first != range.second; ++range.first) 
    {
        range.first->second->OnFinish(task);
    }
    
    if(task.m_Category != Task::CATEGORY_ALL)           
    {   std::list<EventHandler*>& targetList = m_EventSubscribers[task.m_Category];
        std::list<EventHandler*>::iterator iter(targetList.begin());
        while(iter != targetList.end())
        {   (*iter)->OnFinish(task);
            ++iter;
        }
    }
    else
    {
        for(XCr::UInt32 i = 0; i < Task::CATEGORY_MAX; i++)
        {
            std::list<EventHandler*>& targetList = m_EventSubscribers[i];
            std::list<EventHandler*>::iterator iter(targetList.begin());
            while(iter != targetList.end())
            {   (*iter)->OnFinish(task);
                ++iter;
            }
        }
    }    
}