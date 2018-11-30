//
// Created by Rex Lien on 2/22/17.
//

#pragma once

#include <XLN/Core/Config/Config.h>
#include <XLN/Core/Foundation/Task/TaskThread.h>
#include <XLN/Core/Foundation/Fwd.h>

namespace XCr
{

class XLN_CLASS_DECL FrameExecutor : public XCr::TaskThread
{

public:
    FrameExecutor();
    void SetMaxFPS(int fps);
    virtual void add(folly::Func func) override;

protected:

    virtual void OnThreadPreRun() override;
    virtual ~FrameExecutor() override;
    virtual void OnThreadRun() override;
    virtual void OnFrame(XCr::UInt32 deltaTime) = 0;

    XCr::UInt32 m_CurRenderTick;
    XCr::UInt32 m_PrevRenderTick;
    XCr::UInt32 m_FPS;
    XCr::UInt32 m_MillisPerFrame;
    XCr::UInt32 m_CurAccuTime;
    folly::ManualExecutor* m_ManuelExecutor;

};

}