#pragma once

#include <XLN/Core/Config/Config.h>

namespace XGf
{

class SystemFactoryService;

class XLN_CLASS_DECL System
{
public:
    static bool Init(const XCr::String& factoryServiceName);
    static bool Init(XGf::SystemFactoryService* factoryService);
    static void Shutdown();

    static void Main(int argc, char** argv);
    //static void RunOnce();
};
    
}
