#include "CPUThreadPoolExecutor.h"
#include <XLN/Core/Foundation/Task/LifoSemBlockingQueue.h>
#include <XLN/Core/Foundation/Task/TaskAllocator.h>



using namespace XCr;


CPUThreadPoolExecutor::CPUThreadPoolExecutor()
    : m_TaskRunner(std::make_unique<XCr::LifoSemBlockingQueue<Task *>>()),
	m_ThreadFactory(this)
{
    
	m_TaskRunner.SetRunTaskCB(std::bind(&CPUThreadPoolExecutor::RunTask, this, std::placeholders::_1));
	
}


CPUThreadPoolExecutor::~CPUThreadPoolExecutor()
{
    //StopAllThread();
}

void CPUThreadPoolExecutor::RunThread(ThreadPool::ThreadPtr threadRun)
{
    threadRun->m_Idle = false;
    m_TaskRunner.Run();
    threadRun->m_Idle = true;
    m_ThreadPool.AddStoppingThread(threadRun);
}


void CPUThreadPoolExecutor::Post(XCr::Task *task)
{
    m_TaskRunner.PostTask(task);
}


Task *CPUThreadPoolExecutor::AllocateTask() {

    return TaskAllocator::GetInstance().Aquire();

}

void CPUThreadPoolExecutor::DeallocateTask(Task *task)
{
    return TaskAllocator::GetInstance().Release(task);
}

void XCr::CPUThreadPoolExecutor::AddThread(std::size_t threadToAdd)
{
	m_ThreadPool.AddThread(threadToAdd, std::bind(&CPUThreadPoolExecutor::RunThread, this, std::placeholders::_1), &m_ThreadFactory);
}


XCr::CPUThreadPoolExecutor::CPUThreadFactory::CPUThreadFactory(CPUThreadPoolExecutor* executor)
{
	m_Executor = executor;
}

ThreadPool::Thread * XCr::CPUThreadPoolExecutor::CPUThreadFactory::CreateThread()
{
	return XLN_OBJ_NEW CPUExecutorThread(m_Executor);
}

XCr::CPUThreadPoolExecutor::CPUExecutorThread::CPUExecutorThread(ThreadPoolExecutor* executor) : PoolExecutorThread(executor)
{

}

void XCr::CPUThreadPoolExecutor::CPUExecutorThread::Exit()
{
	//since pool excutor thread are treated equally, just posion any random thread
	//CPUThreadPoolExecutor* cpuExecutor = (CPUExecutorThread*)m_Executor;
	XCr::Task* task = ((CPUThreadPoolExecutor*)(m_Executor))->AllocateTask();
	task->SetPosion();
	m_Executor->Post(task);
}
