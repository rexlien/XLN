//
// Created by Rex Lien on 2/22/17.
//

#include "FrameExecutor.h"
#include <folly/executors/ManualExecutor.h>
#include <XLN/Core/Utility/Util.h>
#include <XLN/Core/Foundation/Task/FrameRunner.h>

using namespace XCr;

FrameExecutor::~FrameExecutor()
{
    delete m_ManuelExecutor;
}

FrameExecutor::FrameExecutor()
:TaskThread(XLN_OBJ_NEW FrameRunner())
{

    SetMaxFPS(30);
    m_ManuelExecutor = new folly::ManualExecutor();
}

void FrameExecutor::SetMaxFPS(int fps)
{
    m_FPS = fps;
    m_MillisPerFrame = (1 / (float)m_FPS) * 1000;
}


void FrameExecutor::add(folly::Func func) {

    m_ManuelExecutor->add(std::move(func));
}


void FrameExecutor::OnThreadPreRun() {

    m_PrevRenderTick = XCr::TimeUtil::GetTickCount();
    m_CurAccuTime = 0;
}


void FrameExecutor::OnThreadRun() {


    XCr::UInt32 curTime = XCr::TimeUtil::GetTickCount();

    XCr::UInt32 deltaTime = curTime - m_PrevRenderTick;
    m_CurAccuTime += deltaTime;
    if(m_CurAccuTime > m_MillisPerFrame) {

        TaskThread::OnThreadRun();
        std::chrono::milliseconds millis(m_CurAccuTime);
        m_ManuelExecutor->advance(std::chrono::microseconds(millis));//std::chrono::duration_cast<std::chrono::microseconds>(millis.count()));
        OnFrame(m_CurAccuTime);
        m_CurAccuTime = 0;
    }
    else
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        //usleep(1);
    }

    m_PrevRenderTick = curTime;


}

