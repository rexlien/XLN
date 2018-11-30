#pragma once

#include <XLN/Core/Config/Config.h>
#include <XLN/Core/Foundation/Service.h>
#include <XLN/Core/Framework/Fwd.h>


namespace facebook {
    namespace common {
        namespace mysql_client {

            class AsyncMysqlClient;
            class AsyncMysqlClientFactory;
            template <class TClient, class TClientFactory>
            class ClientPool;
            class AsyncConnectionPool;
            class Query;
            class QueryOperation;
            class QueryResult;
            enum class QueryCallbackReason;
            class ConnectionOptions;
            typedef std::function<void(QueryOperation&, QueryResult*, QueryCallbackReason)>
                QueryCallback;

        }

    }
}



namespace XLN
{
    class XLN_CLASS_DECL MySQLService : public XCr::Service
    {
        XLN_DECLARE_SERVICE_CLASS(MySQLService);
        XLN_DECLARE_CREATOR(MySQLService);
    public:

      
        typedef facebook::common::mysql_client::ClientPool<facebook::common::mysql_client::AsyncMysqlClient, facebook::common::mysql_client::AsyncMysqlClientFactory> AsyncMysqlClientPool;

        
    public:
        virtual bool OnInit() override;
        virtual void GetDependency(std::vector<ServiceID>& dependencies) override;

        
        std::shared_ptr<facebook::common::mysql_client::QueryOperation> Query(const std::string& key, facebook::common::mysql_client::Query& query,
            facebook::common::mysql_client::QueryCallback callback = nullptr, facebook::common::mysql_client::ConnectionOptions* options = nullptr);

    private:
        XGf::ConfigService * m_ConfigService;
        std::unique_ptr<AsyncMysqlClientPool> m_AsyncClientPool;
        std::map<facebook::common::mysql_client::AsyncMysqlClient*, std::shared_ptr<facebook::common::mysql_client::AsyncConnectionPool>> m_ClientonnectionPoolMap;

    };



}
