#pragma once

#include <XLN/Core/Config/Config.h>
#include <XLN/Core/Utility/SharedPtr.h>
#include <XLN/Core/Foundation/Message/IMessage.h>
#include <XLN/Core/Foundation/Task/Task.h>

namespace XCr
{

class TaskProcessor;
class Task;
class Observer;

typedef Task* TaskHandle;

class XLN_CLASS_DECL TaskHandler : public MemObj
{
    public:
        TaskHandler(void);
        virtual ~TaskHandler(void);
       
        void SetTaskProcessor(TaskProcessor* processor); 
        virtual TaskHandle SendTaskAsync(int what, void* param);
        virtual TaskHandle SendTaskAsync(int what, void* param, TaskHandler* target);
        virtual TaskHandle SendTaskAsync(Task::Category category, SharedPtr<IMessage> message, TaskHandler* target);
        virtual void CancelTask(TaskHandle taskHandle);
       
        virtual void Handle(const Task& task);
        virtual void OnFinish(const Task& userData);
        virtual void OnCancel();
         
        void RegisterObserver(Observer* ob);
        void UnRegisterObserver(Observer* ob);
        void SignalObserver(XCr::UInt32 eventID, void* param = 0);
        
        static void RegisterMainProcessor(TaskProcessor* mainProcessor);
        
    protected:
       
       std::list<Observer*> m_Observers;
       TaskProcessor *m_Processor; 
       static TaskProcessor* ms_MainProcessor;
       
        
};

}