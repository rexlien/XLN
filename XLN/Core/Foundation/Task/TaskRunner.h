#pragma once

#include <XLN/Core/Config/Config.h>
#include <boost/thread/condition.hpp>
#include <boost/thread/mutex.hpp>
#include <XLN/Core/Foundation/Fwd.h>
//#include <XLN/Core/Platform/Synchronization/WaitableEvent.h>


namespace XCr
{

class TaskQueue;

class XLN_CLASS_DECL TaskRunner : public XCr::MemObj
{   
    public:
		typedef std::function<void(Task*)> TaskRunCB;
        TaskRunner();
        virtual ~TaskRunner(void);
        virtual void Run();
        virtual void SetFinished();
        virtual void PostTask(Task* task) = 0;
        virtual void PostTaskDelay(Task* task, unsigned long time);

        void WaitTaskComplete();
		void SetRunTaskCB(TaskRunCB cb);

        
    protected:
        virtual void DoRun() = 0;
		TaskRunCB m_RunTaskCB;
        

        
};



}






