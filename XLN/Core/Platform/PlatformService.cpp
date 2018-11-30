#include "PlatformService.h"
#include <boost/filesystem.hpp>
#include <boost/algorithm/string/replace.hpp>
using namespace XCr;

XLN_IMPL_SERVICE_CLASS(PlatformService, XCr::Service);

XCr::String PlatformService::GetAppRoot() const {

    return m_AppRoot;
}

XCr::String PlatformService::GetXLNRoot() const {

    return m_XLNRoot;
}


int PlatformService::GetCPUCount() {
    return 0;
}

XCr::PlatformService::PlatformService()
{

}

bool PlatformService::OnInit()
{
    if(!IsPathExist("XLN"))
    {
        MakeDirectory("XLN");
    }

    if(IsPathExist("XLN")) {
        boost::filesystem::path p(GetWritableRootPath());
        p /= "XLN/";
        m_XLNRoot = p.string();
    }

    if (!IsPathExist("App"))
    {
        MakeDirectory("App");
    }
    if (IsPathExist("App")) {
        boost::filesystem::path p(GetWritableRootPath());
        p /= "App/";
        m_AppRoot = p.string();
    }

    return Service::OnInit();
}


XCr::String PlatformService::GetDeviceModel() {
    return "";
}


XCr::String PlatformService::GetOSName() {
    return "";
}


XCr::String PlatformService::GetNetworkType() {
    return "";
}


XCr::String PlatformService::GetCarrierName() {
    return "";
}

bool PlatformService::IsPathExist(const XCr::String& path) const
{
    boost::filesystem::path subPath(path);
    if(!subPath.is_relative())
    {
        return boost::filesystem::exists(subPath);
    }
    
    boost::filesystem::path targetPath(GetWritableRootPath());
    targetPath /= subPath;
    return boost::filesystem::exists(targetPath);
}

static boost::filesystem::path relativeTo(boost::filesystem::path from, boost::filesystem::path to)
{
    namespace fs = boost::filesystem;
    // Start at the root path and while they are the same then do nothing then when they first
    // diverge take the entire from path, swap it with '..' segments, and then append the remainder of the to path.
    fs::path::const_iterator fromIter = from.begin();
    fs::path::const_iterator toIter = to.begin();

    // Loop through both while they are the same to find nearest common directory
    while (fromIter != from.end() && toIter != to.end() && (*toIter) == (*fromIter))
    {
        ++toIter;
        ++fromIter;
    }

    // Replace from path segments with '..' (from => nearest common directory)
    fs::path finalPath;
    while (fromIter != from.end() && !((*fromIter) == ".") )
    {
        finalPath /= "..";
        ++fromIter;
    }

    // Append the remainder of the to path (nearest common directory => to)
    while (toIter != to.end())
    {
        finalPath /= *toIter;
        ++toIter;
    }

    return finalPath;
}

bool PlatformService::MakeDirectory(const XCr::String& path, bool clearOnExist)
{
    boost::filesystem::path subPath(path);
    
    std::string rootPath = GetWritableRootPath();

    //if absolute, make relative path from it
    if(!subPath.is_relative())
    {
        subPath = relativeTo(rootPath, subPath);
    }

    //if there's a valid subpath;
    if (!subPath.empty())
    {
        boost::filesystem::path dirPath(rootPath);
        dirPath /= subPath;
        if (boost::filesystem::exists(dirPath))
        {   
            if (!clearOnExist)
                return true;
            else
                boost::filesystem::remove_all(dirPath);
        }
        try
        {
            return boost::filesystem::create_directories(dirPath);
        }
        catch (boost::filesystem::filesystem_error& err)
        {
            XLN_LOGW("XLN", err.what());
            return false;
        }
    }
    
    return false;
}

