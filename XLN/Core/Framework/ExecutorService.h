//
// Created by Rex Lien on 2/22/17.
//

#pragma once

#include <XLN/Core/Config/Config.h>
#include <XLN/Core/Foundation/Service.h>
#include <XLN/Core/Foundation/Task/Task.h>
#include <XLN/Core/Framework/Fwd.h>
#include <XLN/Core/Foundation/Fwd.h>
#include <folly/Executor.h>
#include <XLN/Core/Foundation/Message/IMessage.h>
#include <XLN/Core/Foundation/EventExecutor.h>
#include <XLN/Core/Foundation/Fwd.h>

namespace XGf {

class ExecutorService : public XCr::Service
{

XLN_DECLARE_SERVICE_CLASS(ExecutorService)

public:
    ExecutorService();

    virtual ~ExecutorService();
    void PostRenderFrame(folly::Func func);
    void PostRenderFrame(XCr::Task::Category category, XCr::SharedPtr<XCr::IMessage> message);
    RenderFrame* GetRenderFrame();
    XCr::EventExecutor* GetNetworkEventExecutor();



protected:
    virtual bool OnDestroy() override;
    virtual bool OnInit() override;

    RenderFrame* m_RenderFrame;
    std::unique_ptr<XCr::EventExecutor> m_NetworkEventExecutor;
    std::unique_ptr<XCr::IOThreadPoolExecutor> m_IOTasksExecutor;
    std::unique_ptr<XCr::CPUThreadPoolExecutor> m_CPUTasksExecutor;

};

}

