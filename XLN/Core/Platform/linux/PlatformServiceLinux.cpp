#include "PlatformServiceLinux.h"
#include <boost/filesystem.hpp>
#include <boost/dll.hpp>

using namespace XCr;

XLN_IMPL_SERVICE_CLASS(PlatformServiceLinux, XCr::PlatformService);

XCr::String PlatformServiceLinux::GetWritableRootPath() const
{
    char exePath[PATH_MAX];
    ssize_t len = ::readlink("/proc/self/exe", exePath, sizeof(exePath));
    if (len == -1)
        len = 0;
    
    exePath[len] = '\0';
    boost::filesystem::path path(exePath);
    path.remove_filename();

    return path.string() + "/";

}

void XCr::PlatformServiceLinux::CallPlatformMethodImpl(const XCr::String & className, const XCr::String & method, const XCr::String & signature, folly::dynamic & ret, const folly::dynamic & object)
{
    //no platfrom call for linux
}
