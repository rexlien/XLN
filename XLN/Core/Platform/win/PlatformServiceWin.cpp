#include "PlatformServiceWin.h"
#include <Windows.h>

using namespace XCr;

XLN_IMPL_SERVICE_CLASS(PlatformServiceWin, XCr::PlatformService);

XCr::String PlatformServiceWin::GetWritableRootPath() const
{
    HMODULE hModule = GetModuleHandleW(NULL);
    TCHAR path[MAX_PATH];
    GetModuleFileName(hModule, path, MAX_PATH);
    std::string::size_type pos = std::string(path).find_last_of("\\/");
    
    return std::string(path).substr(0, pos + 1);

}

void XCr::PlatformServiceWin::CallPlatformMethodImpl(const XCr::String & className, const XCr::String & method, const XCr::String & signature, folly::dynamic & ret, const folly::dynamic & object)
{
    //no platfrom call for windows
}
