#include "ServiceMgr.h"
#include "Service.h"

#include <XLN/Core/Foundation/Log.h>
#include <algorithm>

#undef min

using namespace XCr;

ServiceMgr* ServiceMgr::ms_UserServiceMgr = 0;
std::map<XCr::String, XCr::ServiceMgr*> ServiceMgr::ms_ServiceMgrs;
XCr::ServiceMgr* ServiceMgr::ms_ActiveServiceMgr = 0;

XCr::ServiceMgr* XCr::ServiceMgr::GetActiveServiceMgr()
{
    return ms_ActiveServiceMgr;
}

void XCr::ServiceMgr::AddServiceMgr( const XCr::String& name, XCr::ServiceMgr* serviceMgr )
{
    ms_ServiceMgrs[name] = serviceMgr;
}

void XCr::ServiceMgr::SetActiveServiceMgr( const XCr::String& name )
{
    std::map<XCr::String, XCr::ServiceMgr*>::iterator iter(ms_ServiceMgrs.find(name));
    if(iter != ms_ServiceMgrs.end())
    {   ms_ActiveServiceMgr = iter->second;
    }
}

XCr::UInt32
GetCurrentTick()
{
    using namespace std::chrono;
    return duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
}


ServiceMgr::ServiceMgr(void)
{
    //ms_CoreServiceMgr = this;
    m_Initialized = false;
    m_LastTime = GetCurrentTick();
    
    
}

ServiceMgr::~ServiceMgr(void)
{
    ShutDown();
}
/*
void XGf::ServiceMgr::RegisterService( ServiceID id, Service* service )
{
    m_Services[id] = service;
    service->OnPreInit();
    
    if(m_Initialized)
        service->OnInit();
}
*/



void XCr::ServiceMgr::RegisterService( Service* service )
{
    Service::ServiceID id = service->GetID();//Service::ServiceID)(&typeid(*service));
    
    //check if service already registered  
    ServiceMap::iterator iter(m_Services.find(id));
    if(iter != m_Services.end())
    {   XLN_LOGW("Service laready registered\n");
        return;
    } 
        
    service->SetServiceMgr(this);
    service->OnPreInit();

    if(m_Initialized)
    {    service->OnInit(); 
    }
    
    ServiceInfo info;
    info.m_Service = service;
    service->GetDependency(info.m_Dep);
    //dependnecy will be considered only before init
    if(!m_Initialized)
    {   
        info.m_Order = -1;
    }
    else
    {   info.m_Order = std::numeric_limits<XCr::Int32>::infinity();
        ServiceOpInfo opInfo;
        opInfo.m_Service = service;
        opInfo.m_ServiceID = id;
        m_ServiceOpList.push_back(opInfo);  
    }
    m_Services[id] = info;
    
    do
    {   
        m_FastSearchMap[id] = service;
        id = id->GetBaseRTTI(); 
        
    }while(id);
}

void ServiceMgr::RegisterService(const XCr::String &name)
{
    std::map<XCr::Service::ServiceID, folly::Function<Service*(void)>>& creators = Service::GetCreators();

    std::map<XCr::Service::ServiceID, folly::Function<Service*(void)>>::iterator iter = creators.begin();
    folly::Function<Service*(void)> creator;
    while(iter != creators.end())
    {
        if(iter->first->GetName() == name)
        {
            RegisterService((iter->second)());

        }
        ++iter;
    }
}


void XCr::ServiceMgr::Init()
{   
    m_Initialized = true;
    
    //build service dependency registered 
    BuildDep();
    std::list<ServiceOpInfo>::iterator iter(m_ServiceOpList.begin());
    while(iter != m_ServiceOpList.end())
    {
        (*iter).m_Service->OnInit();
        ++iter;
    }
    
}

void XCr::ServiceMgr::RunOnce()
{
    if(m_Initialized)
    {   
        XCr::UInt32 lastTime =  m_LastTime;
        m_LastTime = GetCurrentTick();
        float diff = (m_LastTime - lastTime) / (float)1000;
        m_AccTime += diff;
        std::list<ServiceOpInfo>::iterator iter(m_ServiceOpList.begin());
        while(iter != m_ServiceOpList.end())
        {
            (*iter).m_Service->OnTick(diff);
            ++iter;
        }
        
        iter = m_ServiceOpList.begin();
        while(iter != m_ServiceOpList.end())
        {
            (*iter).m_Service->OnRender();
            ++iter;
        }
    }
    
}

void XCr::ServiceMgr::ShutDown()
{
    std::list<ServiceOpInfo>::reverse_iterator iter(m_ServiceOpList.rbegin());
    //char buf[255];
    //std::string logString;
    while(iter != m_ServiceOpList.rend())
    {   
        //sprintf(buf, "Destroying Service: %s\n", (*iter).m_Service->GetServiceName().c_str());
        //logString += buf;
        (*iter).m_Service->OnDestroy();
        XLN_DELETE (*iter).m_Service;
        ++iter;
    }
    
    m_ServiceOpList.clear();
    m_Services.clear();
    m_FastSearchMap.clear();
   // XLN_LOGW(logString.c_str());
}

void XCr::ServiceMgr::Run()
{
    Init();
    //while(true)
    {
        RunOnce();
    }
    ShutDown();
}

XCr::Float32 XCr::ServiceMgr::GetTime() const
{
    return m_AccTime;
}

ServiceMgr* XCr::ServiceMgr::GetUserServiceMgr()
{
    return ms_UserServiceMgr;
}

void XCr::ServiceMgr::SetUserServiceMgr( ServiceMgr* serviceMgr )
{
    ms_UserServiceMgr = serviceMgr;
}

static XCr::Int32 RecurrsiveBuildDep(const std::vector<XCr::Service::ServiceID>& parentIDs, XCr::ServiceMgr::ServiceInfo& info, ServiceMgr::ServiceMap& infoMap)
{
    const std::vector<XCr::Service::ServiceID>& dep = info.m_Dep;
    
    //if it's been found
    if(info.m_Order != -1)
    {
        return info.m_Order;
    }
    else if(dep.empty())
    {
        return 0;
    }
    else
    {   XCr::Int32 maxDep = -1;//std::numeric_limits<XCr::Int32>::min();
        for(std::size_t i = 0; i < dep.size(); i++)
        {   
            //check acyclic dependency
            for(std::size_t j = 0; j < parentIDs.size(); j++)
            {
                if(parentIDs[j] == dep[i])
                {
                    XLN_LOGE("ServiceMgr", "Crash Error! Acyclic dependencies found\n");
                    return -1;
                }
                
            }


            XCr::Service::ServiceID depID = dep[i];
            ServiceMgr::ServiceMap::iterator iter = std::find_if(infoMap.begin(), infoMap.end(), [&depID](ServiceMgr::ServiceMap::value_type& arg) {

                if(arg.first == depID)
                    return true;
                XCr::Service::ServiceID baseID = arg.first->GetBaseRTTI();
                while(baseID)
                {
                    if(baseID == depID)
                        return true;

                    baseID = baseID->GetBaseRTTI();
                }

                return false;
            });
            //find dependency's order
            //ServiceMgr::ServiceMap::iterator iter(infoMap.find(dep[i]));
            if(iter != infoMap.end())
            {   std::vector<XCr::Service::ServiceID> ids(parentIDs);
                ids.push_back(dep[i]);
                XCr::Int32 curDep = 1 + RecurrsiveBuildDep(ids, iter->second, infoMap);
                if(maxDep < curDep)
                    maxDep = curDep;
                
            }
            else
            {   //this should crash!
                XLN_LOGE("ServiceMgr", "Crash Error! One of Dependencies %s not Registered\n", info.m_Service->GetServiceName().c_str());
                return -1;
            }
           // 1 + 
        }
        info.m_Order = maxDep;
        return maxDep;
    }
}

void XCr::ServiceMgr::BuildDep()
{   
    ServiceMap::iterator iter(m_Services.begin());
    while(iter != m_Services.end())
    {
       std::vector<XCr::Service::ServiceID> rootID;
       rootID.push_back(iter->first);
       RecurrsiveBuildDep(rootID, iter->second, m_Services);
       ++iter;
    }
    
    
     iter = m_Services.begin();
     while(iter != m_Services.end())
     {  ServiceOpInfo opInfo;
        opInfo.m_Order = iter->second.m_Order;
        opInfo.m_ServiceID = iter->first;
        opInfo.m_Priority = -1;
        opInfo.m_Service = iter->second.m_Service;
        m_ServiceOpList.push_back(opInfo);
        ++iter;
     }
     m_ServiceOpList.sort();
     
     std::list<ServiceOpInfo>::iterator opIter(m_ServiceOpList.begin());
     while(opIter != m_ServiceOpList.end())
     {
        XLN_LOGD("OpOrder: %s \n", (*opIter).m_Service->GetServiceName().c_str());
        ++opIter;
     }
    
    //build op list
       
}

void XCr::ServiceMgr::SetVirtualProcessName( const XCr::String& name )
{
    m_ProcessName = name;
}

const XCr::String& XCr::ServiceMgr::GetVirtualProcessName() const
{
    return m_ProcessName;
}

const std::map<XCr::String, XCr::ServiceMgr*>& XCr::ServiceMgr::GetServiceMgrs()
{
    return ms_ServiceMgrs;
}
/*
void XCr::ServiceMgr::UnRegisterService( const XCr::Util::UniqueID& uid )
{
    //m_Services.erase(uid);    
}
*/
void XCr::ServiceMgr::UnRegisterService( Service* service )
{
    Service::ServiceID id = service->GetID();
    m_Services.erase(id);
    do
    {      
        m_FastSearchMap.erase(id);
        id = id->GetBaseRTTI(); 

    }while(id);
    
}
XCr::ServiceMgr::ServiceOpInfo::ServiceOpInfo()
{
    m_Order = std::numeric_limits<XCr::Int32>::infinity();
    m_Service = 0;
    m_ServiceID = 0;//boost::uuids::nil_uuid();
    m_Priority = -1;
}