//
// Created by Rex Lien on 3/2/17.
//

#include "RenderFrame.h"
#include <XLN/Core/Framework/System.h>
#include <XLN/Core/Foundation/ServiceMgr.h>

using namespace XGf;

RenderFrame::RenderFrame()
{
    SetTaskProcessor(this);
}

void RenderFrame::OnFrame(XCr::UInt32 deltaTime)
{
    //System::RunOnce();
    XCr::ServiceMgr::GetActiveServiceMgr()->RunOnce();
}

