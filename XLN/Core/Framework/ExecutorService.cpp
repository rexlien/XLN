//
// Created by Rex Lien on 2/22/17.
//

#include "ExecutorService.h"
#include <XLN/Core/Utility/Util.h>
#include <XLN/Core/Framework/System.h>
#include <XLN/Core/Framework/RenderFrame.h>
#include <XLN/Core/Foundation/EventExecutor.h>
#include <XLN/Core/Foundation/Executor/CPUThreadPoolExecutor.h>
#include <XLN/Core/Foundation/Executor/IOThreadPoolExecutor.h>

using namespace XGf;
using namespace XCr;

XLN_IMPL_SERVICE_CLASS(ExecutorService, XCr::Service);


ExecutorService::ExecutorService()
    :m_NetworkEventExecutor(XLN_OBJ_NEW EventExecutor()),
    m_CPUTasksExecutor(XLN_OBJ_NEW CPUThreadPoolExecutor()),
    m_IOTasksExecutor(XLN_OBJ_NEW IOThreadPoolExecutor())
{
    m_RenderFrame = XLN_OBJ_NEW XGf::RenderFrame();
    m_CPUTasksExecutor->SetMaxThread(4);


}

ExecutorService::~ExecutorService()
{

    delete m_RenderFrame;
}


bool ExecutorService::OnInit()
{
    m_RenderFrame->Start();
    m_RenderFrame->SetMaxFPS(30);
    m_NetworkEventExecutor->Start();

    return Service::OnInit();
}

void ExecutorService::PostRenderFrame(folly::Func func)
{
    m_RenderFrame->add(std::move(func));
}


void ExecutorService::PostRenderFrame(XCr::Task::Category category,
                                      XCr::SharedPtr<XCr::IMessage> message)
{

    m_RenderFrame->SendTaskAsync(category, message, m_RenderFrame);

}


RenderFrame *ExecutorService::GetRenderFrame() {
    return m_RenderFrame;
}

XCr::EventExecutor* ExecutorService::GetNetworkEventExecutor()
{
    return m_NetworkEventExecutor.get();
}


bool ExecutorService::OnDestroy()
{

    m_RenderFrame->Stop();
    m_CPUTasksExecutor->StopAllThread();
    return Service::OnDestroy();
}


