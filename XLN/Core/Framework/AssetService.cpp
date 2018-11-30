#include <XLN/Core/Framework/AssetService.h>
#include <XLN/Core/Platform/PlatformService.h>
#include <XLN/Core/Foundation/ServiceMgr.h>

using namespace XGf;

XLN_IMPL_SERVICE_CLASS(AssetService, XCr::Service);

AssetService::AssetService()
{
    
}

void XGf::AssetService::SetAssetPath(const std::string & root)
{
    PathMap path;
    path.m_Name = "Asset";
    path.m_Root = root;
    AddPathMap(path);
    //m_VirtualPaths[path.m_Name] = path;
}

void XGf::AssetService::AddPathMap(const PathMap & path)
{
    m_VirtualPaths[path.m_Name] = path;
}

std::string XGf::AssetService::ResolveFilePath(const std::string & name, const std::string & file)
{
    auto iter = m_VirtualPaths.find(name);
    if (iter != m_VirtualPaths.end())
    {
        return m_PlatformService->GetAppRoot() + "/" + iter->second.m_Root + "/" + file;
    }

    return "";
}

void XGf::AssetService::GetDependency(std::vector<ServiceID>& dependencies)
{
    dependencies.push_back(XCr::Service::GetServiceID<XCr::PlatformService>());
}

bool XGf::AssetService::OnInit()
{
    m_PlatformService = XCr::ServiceMgr::GetActiveServiceMgr()->GetService<XCr::PlatformService>();
    return true;
}
