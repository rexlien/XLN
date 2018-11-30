//
// Created by Rex Lien on 3/2/17.
//

#pragma once

#include <XLN/Core/Config/Config.h>
#include <XLN/Core/Foundation/FrameExecutor.h>
#include <XLN/Core/Foundation/Task/EventDispatcher.h>

namespace XGf
{

class RenderFrame : public XCr::FrameExecutor, public XCr::EventDispatcher
{


protected:
public:
    RenderFrame();

    virtual void OnFrame(XCr::UInt32 deltaTime) override;
};

}

