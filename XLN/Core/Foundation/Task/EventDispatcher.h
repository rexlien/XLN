#pragma once

#include <XLN/Core/Config/Config.h>
#include <XLN/Core/Foundation/Task/EventHandler.h>
#include <boost/unordered_map.hpp>

namespace XCr
{

class EventHandler;

class XLN_CLASS_DECL EventDispatcher : public TaskHandler
{
    public:
       
        EventDispatcher(void);
        ~EventDispatcher(void);
      
        void SubscribeEvent(EventHandler* handler);
        void DeSubscribeEvent(EventHandler* handler);
        virtual void Handle(const Task& task);
        virtual void OnFinish(const Task& task);


        template<class T>
        XCr::SharedPtr<T> AcquireEventHandler()
        {

            XCr::SharedPtr<T> newEventHandler = XCr::SharedPtr<T>(XLN_OBJ_NEW T());
            newEventHandler->SubscribeEvent(this);
            return newEventHandler;
        }

        template<class T, class U>
        XCr::SharedPtr<T> AcquireEventHandler(U param1)
        {
            XCr::SharedPtr<T> newEventHandler = XCr::SharedPtr<T>(XLN_OBJ_NEW T(param1));
            newEventHandler->SubscribeEvent(this);
            return newEventHandler;
        }

        template<class T, class T1, class T2>
        XCr::SharedPtr<T> AcquireEventHandler(T1 param1, T2 param2)
        {
            XCr::SharedPtr<T> newEventHandler = XCr::SharedPtr<T>(XLN_OBJ_NEW T(param1, param2));
            newEventHandler->SubscribeEvent(this);
            return newEventHandler;
        }


    private:
        //std::vector<>
        typedef boost::unordered_multimap<XCr::Util::UniqueID, EventHandler*, XCr::Util::UIDHasher> EventHandlerHash;
        EventHandlerHash m_EventHandlers; 
        std::vector<std::list<EventHandler*> > m_EventSubscribers;

};



}