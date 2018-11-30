#pragma once

#include <XLN/Core/Config/Config.h>
#include <boost/thread/mutex.hpp>
#include <boost/thread/tss.hpp>

namespace XCr
{

class Task;
class TaskQueue;

typedef Task* TaskHandle;

extern XLN_CLASS_DECL int GetTaskID(TaskHandle handle);

class XLN_CLASS_DECL TaskProcessor : public MemObj
{
    public:
        //static TaskProcessor& GetMainProcessor();
        TaskProcessor(void);
        ~TaskProcessor(void);
        virtual void Init();
        //virtual void Execute();
        
        virtual TaskHandle SubmitTask(Task* task);
        //virtual void CancelTask(TaskHandle task);
        virtual void CleanTasks();
        virtual void Free();
        static TaskProcessor* GetCurrent();
        
    protected:
        
        
        static void ResetCurrent(TaskProcessor* current);
        
        static boost::thread_specific_ptr<TaskProcessor> m_Current;
       
        boost::mutex m_WorkQueueReloadMutex;
        XCr::TaskQueue* m_IncomingQueue;
        XCr::TaskQueue* m_WorkingQueue;
};


//extern XLN_CLASS_DECL TaskProcessor g_MainProcessor;

}