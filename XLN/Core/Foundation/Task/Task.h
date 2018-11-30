#pragma once

#include <XLN/Core/Config/Config.h>
#include <XLN/Core/Foundation/Message/IMessage.h>
#include <XLN/Core/Utility/SharedPtr.h>
#include <folly/Function.h>

namespace XCr
{

class TaskHandler;
class TaskProcessor;

class XLN_CLASS_DECL Task : public MemObj
{
    public:
        enum Category
        {
            CATEGORY_SYSTEM,
            CATEGORY_USER,
            CATEGORY_ALL,
            CATEGORY_MAX
            
        };
        
        enum SystemAction
        {
            SA_NONE,
            SA_QUIT_RUNNER,
            SA_RELEASE_TASK,
            SA_WAIT_NOTIFIER,
            SA_TIMER
            
        };
        
        enum TaskState
        {
            TS_INVALID,
            TS_QUEUED,
            TS_CANCELD,
            TS_HANDLED
        };
        
        //class CategoryInfo
        //{
        //    public:
        //        //CategoryInfo();
        //        //static CategoryInfo
        //        //XCr::UInt32 m_Flag;
        //};
       
        
        Task();
        //Task(int what, int when, void* param, TaskHandler* target);
        Task(folly::Function<void()> follyFunc);
        ~Task(void);
        
        void Cancel();
        bool IsCanceled() const;
        
        int m_What;
        int m_When;
        void* m_UserParam;
        
        //private use only
        SystemAction m_SysAction;
        TaskProcessor* m_CurrentProcessor;
        
        /////////////////
        boost::any m_Any;
        TaskHandler* m_Whom;
        Category m_Category;
        XCr::SharedPtr<IMessage> m_Message;
        folly::Function<void()> m_FollyFunc;
        folly::Function<void(Task*)> m_DeleteCB;

        int GetID() const;
        void Reset();
        void SetState(TaskState state);
        void SetPosion();
        TaskState GetState() const;
        
   private:
        int m_ID;
        TaskState m_State;
};




}