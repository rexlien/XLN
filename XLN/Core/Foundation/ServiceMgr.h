#pragma once

#include <XLN/Core/Config/Config.h>
#include <XLN/Core/Foundation/Service.h>
#include <functional>

namespace XCr
{

class XLN_CLASS_DECL ServiceMgr : public MemObj
{
    public:
        class ServiceInfo
        {

        public:
            ServiceInfo()
            {
                m_Order = -1;
                m_Service = 0;
            }
            ~ServiceInfo()
            {
                m_Service = 0;
            }

            std::vector<Service::ServiceID> m_Dep;
            XCr::Int32 m_Order;
            Service* m_Service;

        };
        
        
        typedef std::map<XCr::Service::ServiceID, ServiceInfo> ServiceMap;
       
        
        class ServiceOpInfo
        {
            public:
                ServiceOpInfo();
            
                Service::ServiceID m_ServiceID;
                XCr::Int32 m_Order;     
                XCr::Int32 m_Priority;
                Service* m_Service;
                
                bool operator <(const ServiceOpInfo& rhs) const
                {
                    return m_Order < rhs.m_Order;
                }
            
        };
        
        
        
        ServiceMgr(void);
        virtual ~ServiceMgr(void);
        
        void RegisterService(Service* service);
        void RegisterService(const XCr::String& name);
        //void UnRegisterService(const XCr::Util::UniqueID& uid);
        void UnRegisterService(Service* service);
        template<class T>
        void UnRegisterService()
        {   
            Service::ServiceID id = Service::GetServiceID<T>();
            ServiceMap::iterator iter(m_Services.find(id));
            if(iter != m_Services.end())
            {
                m_Services.erase(iter);
                delete (iter->second).m_Service;
            }
            
            do
            {      
                m_FastSearchMap.erase(id);
                id = id->GetBaseRTTI(); 

            }while(id);
        }
        
        template<class T>
        T* GetService()
        {
            T* ret = 0;
            Service::ServiceID id = Service::GetServiceID<T>();//(Service::ServiceID)(&typeid(T));
            std::map<XCr::Service::ServiceID, Service*>::iterator iter(m_FastSearchMap.find(id));
            if(iter != m_FastSearchMap.end())
            {
                ret = (T*)(iter->second);
            }  
            
            return ret;
        }  
        virtual void Init();
        virtual void ShutDown();
        virtual void RunOnce();
        virtual void Run();
        void SetVirtualProcessName(const XCr::String& name);
        const XCr::String& GetVirtualProcessName() const;
        XCr::Float32 GetTime() const;
        
        static ServiceMgr* GetUserServiceMgr();
        static void SetUserServiceMgr(ServiceMgr* serviceMgr);

        static XCr::ServiceMgr* GetActiveServiceMgr();
        static void AddServiceMgr(const XCr::String& name, XCr::ServiceMgr* serviceMgr);
        static void SetActiveServiceMgr(const XCr::String& name);
        static const std::map<XCr::String, XCr::ServiceMgr*>& GetServiceMgrs();
   protected:    
        
        virtual void BuildDep();       
        
        static ServiceMgr* ms_UserServiceMgr;
        static std::map<XCr::String, XCr::ServiceMgr*> ms_ServiceMgrs;
        static XCr::ServiceMgr* ms_ActiveServiceMgr;
        
        
        ServiceMap m_Services;  
        std::map<XCr::Service::ServiceID, Service*> m_FastSearchMap;
        std::list<ServiceOpInfo> m_ServiceOpList;
        bool m_Initialized;
        XCr::UInt32 m_LastTime;
        XCr::Float32 m_AccTime;
        XCr::String m_ProcessName;
        
       
};

}