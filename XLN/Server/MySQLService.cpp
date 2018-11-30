#include "MySQLService.h"
#include <XLN/Core/Framework/ConfigService.h>
#include <XLN/Core/Foundation/ServiceMgr.h>
#include <squangle/mysql_client/AsyncMysqlClient.h>
#include <squangle/mysql_client/Query.h>
#include <squangle/mysql_client/ClientPool.h>
#include <squangle/mysql_client/AsyncConnectionPool.h>
#include <nlohmann/json.hpp>


using namespace XLN;
using namespace facebook::common::mysql_client;

XLN_IMPL_SERVICE_CLASS(MySQLService, XCr::Service);
XLN_IMPLEMENT_CREATOR(MySQLService);

static std::map<std::string, ConnectionKey> s_ConnectionKeys;

bool XLN::MySQLService::OnInit()
{
    m_ConfigService = XCr::ServiceMgr::GetActiveServiceMgr()->GetService<XGf::ConfigService>();
    int poolCount = 1;
    try
    {
        poolCount = m_ConfigService->GetAppConfig()["XLNAppConfig"]["ServerConfigs"]["MySQLService"]["ThreadCount"];
    }
    catch (...)
    {

    }
   
    m_AsyncClientPool =  std::make_unique<AsyncMysqlClientPool>(
        std::make_unique<AsyncMysqlClientFactory>(),
        poolCount);

    m_AsyncClientPool->forEachClient([this](auto client) { 
        
        PoolOptions options;
        
        auto connectionPool = AsyncConnectionPool::makePool(client, options);
        this->m_ClientonnectionPoolMap.insert(std::make_pair(client.get(), connectionPool));

    });

    try
    {
        auto connections = m_ConfigService->GetAppConfig()["XLNAppConfig"]["ServerConfigs"]["MySQLService"]["DBConnections"];
        if (connections != nullptr)
        {
            for (nlohmann::json::iterator it = connections.begin(); it != connections.end(); ++it) 
            {
               ConnectionKey connectionKey((*it)["Host"].get<std::string>(), (*it)["Port"], (*it)["DBName"].get<std::string>(), (*it)["User"].get<std::string>(), (*it)["PW"].get<std::string>());
               s_ConnectionKeys.insert(std::make_pair((*it)["Key"].get<std::string>(), connectionKey));
                
            }
        }
    }
    catch (...)
    {

    }
    
    return true;
}

void XLN::MySQLService::GetDependency(std::vector<ServiceID>& dependencies)
{
    
}

std::shared_ptr<QueryOperation> XLN::MySQLService::Query(const std::string& key, facebook::common::mysql_client::Query& query, QueryCallback callback, 
        facebook::common::mysql_client::ConnectionOptions* options)
{
    auto keyIter = s_ConnectionKeys.find(key);
    if (keyIter == s_ConnectionKeys.end())
    {
        return 0;
    }
    
    auto client = m_AsyncClientPool->getClient(key);
   
    auto poolIter = m_ClientonnectionPoolMap.find(client.get());
    if (poolIter == m_ClientonnectionPoolMap.end())
    {
        return 0;
    }

    ConnectionKey connecitonKey = keyIter->second;
    auto connecitonPool = poolIter->second;


    std::unique_ptr<Connection> connection;
    try
    {
        if(options)
            connection = connecitonPool->connect(connecitonKey.host, connecitonKey.port, connecitonKey.db_name, connecitonKey.user, connecitonKey.password);
        else
            connection = connecitonPool->connect(connecitonKey.host, connecitonKey.port, connecitonKey.db_name, connecitonKey.user, connecitonKey.password, *options);
    }
    catch (MysqlException& ex)
    {
        XLN_LOGE("sql connect exception:", ex.what());
        return 0;
    } 
    
    
    std::shared_ptr<QueryOperation> queryOp = Connection::beginQuery(std::move(connection), query);
    if (callback)
        queryOp->setCallback(callback);
    queryOp->run();

    return queryOp;
   
}

