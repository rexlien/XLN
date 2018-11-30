#include "FoundationSDM.h"
#include <XLN/Core/Foundation/ServiceMgr.h>


using namespace XCr;

ServiceMgr* FoundationSDM::ms_CoreServiceMgr = 0;

void XCr::FoundationSDM::Init()
{
    ms_CoreServiceMgr =  XLN_OBJ_NEW ServiceMgr();
}

void XCr::FoundationSDM::ShutDown()
{
    XLN_SAFE_DELETE(ms_CoreServiceMgr);
    ms_CoreServiceMgr = 0;
    
}

ServiceMgr* XCr::FoundationSDM::GetCoreServiceMgr()
{
    return ms_CoreServiceMgr;
}
