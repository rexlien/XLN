#include "Assert.h"

using namespace XCr;
XCr::AssertFailProcFuncPtr XCr::Assert::ms_pfnNiAssertFailProc = 0;//Assert::DefaultAssertFail;


void XCr::Assert::RegisterAssertFailCB( AssertFailProcFuncPtr cb )
{
    ms_pfnNiAssertFailProc = cb;    
}