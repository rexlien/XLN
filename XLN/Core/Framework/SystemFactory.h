//
// Created by Rex Lien on 3/23/17.
//

#pragma once

#include <XLN/Core/Config/Config.h>
#include <XLN/Core/DebugHelper/MemObj.h>

namespace XGf {

class Application;

class SystemFactory : XCr::MemObj
{
public:
    virtual ~SystemFactory();
    virtual Application* CreateApplication() = 0;
};

}
