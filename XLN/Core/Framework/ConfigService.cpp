#include <XLN/Core/Foundation/ServiceMgr.h>
#include <XLN/Core/Framework/ConfigService.h>
#include <XLN/Core/Framework/AssetService.h>

using namespace XGf;

XLN_IMPL_SERVICE_CLASS(ConfigService, XCr::Service);


XGf::ConfigService::ConfigService()
{
  
    
}

bool XGf::ConfigService::OnInit()
{   
   
    m_AssetService = XCr::ServiceMgr::GetActiveServiceMgr()->GetService<XGf::AssetService>();
    AddConfiguration("XLN", "XLNConfig.json");
    return true;
}

void XGf::ConfigService::AddConfiguration(const std::string & key, const std::string & filename)
{
    nlohmann::json json = nullptr;
    try
    {
        std::ifstream fstream(m_AssetService->ResolveFilePath("Asset", filename), std::ifstream::in);
        json = nlohmann::json::parse(fstream);

    }
    catch (std::exception& ex)
    {
        XLN_LOGE("Config load error: %s", ex.what());
        throw ex;
    }
    m_Configs.insert(std::make_pair(key, json));

}

const nlohmann::json XGf::ConfigService::GetConfig(const std::string & key) const
{
    std::map<std::string, nlohmann::json>::const_iterator iter = m_Configs.find(key);
    if (iter != m_Configs.end())
    {
        return iter->second;
    }
    return 0;

}

const nlohmann::json XGf::ConfigService::GetAppConfig() const
{
    
    return GetConfig("XLN");
}

void XGf::ConfigService::GetDependency(std::vector<ServiceID>& dependencies)
{
    dependencies.push_back(Service::GetServiceID<AssetService>());
}
