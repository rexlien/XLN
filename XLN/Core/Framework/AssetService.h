#include <XLN/Core/Config/Config.h>
#include <XLN/Core/Foundation/Service.h>
#include <XLN/Core/Platform/Fwd.h>

namespace XGf
{

    class XLN_CLASS_DECL AssetService : public XCr::Service
    {
        XLN_DECLARE_SERVICE_CLASS(AssetService);
       

    public:
        AssetService();
        class PathMap
        {
        public:
            std::string m_Name;
            std::string m_Root;
        };

        void SetAssetPath(const std::string& root);
        virtual void AddPathMap(const PathMap& path);
        virtual std::string ResolveFilePath(const std::string& name, const std::string& file);

        virtual void GetDependency(std::vector<ServiceID>& dependencies);;
        virtual bool OnInit() override;;
        

    protected:
        
        std::map<std::string, PathMap> m_VirtualPaths;
        XCr::PlatformService* m_PlatformService;
        

    };


}