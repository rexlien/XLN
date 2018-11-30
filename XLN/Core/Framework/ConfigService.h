#include <XLN/Core/Config/Config.h>
#include <XLN/Core/Foundation/Service.h>
#include <XLN/Core/Framework/Fwd.h>

#include <nlohmann/json.hpp>


namespace XGf
{
       
    class ConfigService : public XCr::Service
    {
        XLN_DECLARE_SERVICE_CLASS(ConfigService);

    public:
        ConfigService();
        virtual bool OnInit() override;

        void AddConfiguration(const std::string& key, const std::string& filename);
        const nlohmann::json GetConfig(const std::string& key) const;
        const nlohmann::json GetAppConfig() const;

        virtual void GetDependency(std::vector<ServiceID>& dependencies) override;
    private:
        std::map<std::string, nlohmann::json> m_Configs;
        XGf::AssetService* m_AssetService;

    };


}