#pragma once

#include <XLN/Core/Config/Config.h>
#include <XLN/Core/Foundation/Task/TaskHandler.h>
#include <XLN/Core/Foundation/Task/Task.h>
#include <XLN/Core/Utility/SharedPtr.h>

namespace XCr
{

class EventDispatcher;

class XLN_CLASS_DECL EventHandler : public TaskHandler
{
    public:
        typedef std::list<XCr::Util::UniqueID> EventFilterList;
        EventHandler();
        ~EventHandler();
        
        void SetEventDispatcher(EventDispatcher* eventMgr);
        void SubscribeEvent();
        void DeSubscribeEvent();
        void SubscribeEvent(EventDispatcher* eventHandlerMgr);
        void BroadcastTask(XCr::Int32 what, void* param, 
            Task::Category category = Task::CATEGORY_ALL, SharedPtr<IMessage> message = SharedPtr<IMessage>());
        void BroadcastTaskAsync(XCr::Int32 what, void* param);
        virtual Task::Category GetCategory() const;
        virtual const EventFilterList* GetEventFilterClassID() const;
        
        virtual void Handle(const Task& task);
        
    protected:
        virtual void OnHandleMessage(SharedPtr<IMessage> message) {};
        
    private:
        EventDispatcher* m_EventHandlerMgr;


};

/************************************************************************/
/* helper to forward handler message to given parent class                                                                    
/************************************************************************/
template <class T>
class EventHandlerHelper : public XCr::EventHandler
{   
    public:
        EventHandlerHelper(T* parentclass)
        {
            m_ParentClass = parentclass;
        }
        virtual void OnHandleMessage(XCr::SharedPtr<XCr::IMessage> message)
        {
            m_ParentClass->OnHandleMessage(message);
        }
    private:
        T* m_ParentClass;

};

template <class T, class U, class X>
struct FilterSetter
{
    static void Set(XCr::EventHandler::EventFilterList& filterID)
    {
        filterID.push_back(T::ClassID());
        filterID.push_back(U::ClassID());
        filterID.push_back(X::ClassID());

    }
};

template <class T, class U>
struct FilterSetter<T, U, XCr::IMessage>
{
    static void Set(XCr::EventHandler::EventFilterList& filterID)
    {
        filterID.push_back(T::ClassID());
        filterID.push_back(U::ClassID());

    }
};

template <class T>
struct FilterSetter<T, XCr::IMessage, XCr::IMessage>
{
    static void Set(XCr::EventHandler::EventFilterList& filterID)
    {
        filterID.push_back(T::ClassID());
        
    }
};


template <class T, class MessageType1, class MessageType2 = XCr::IMessage, class MessageType3 = XCr::IMessage>
class MessageHandlerHelper : public XCr::EventHandler
{
public:
    MessageHandlerHelper(T* parentclass)
    {
        m_ParentClass = parentclass;
        SetFilter<MessageType1, MessageType2, MessageType3>();
    }
    
    template <class X, class Y, class Z>
    void SetFilter()
    {   
        FilterSetter<X, Y, Z>::Set(m_EventFilter);
    }
    
    virtual void OnHandleMessage(XCr::SharedPtr<XCr::IMessage> message)
    {
        m_ParentClass->OnHandleMessage(message);
    }
    virtual const XCr::EventHandler::EventFilterList* GetEventFilterClassID() const
    {
        return &m_EventFilter;
    }
private:
    T* m_ParentClass; 
    EventFilterList m_EventFilter;
};


}