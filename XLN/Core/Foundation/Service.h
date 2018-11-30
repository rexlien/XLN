#pragma once

#include <XLN/Core/Config/Config.h>
#include <boost/uuid/uuid_generators.hpp>
#include <XLN/Core/Utility/RTTI.h>
#include <folly/Function.h>
#include <XLN/Core/Foundation/Log.h>

#define XLN_DECLARE_SERVICE_CLASS(ServiceClass) \
    private: \
public: \
    static XCr::String GetName() {return ms_RTTI.GetName();}; \
    virtual ServiceID GetID() const override; \
    virtual XCr::String GetServiceName() const override; \
    XLN_DECLARE_RTTI

#define XLN_IMPL_SERVICE_CLASS(ServiceClass, baseclassname) \
    XCr::Service::ServiceID ServiceClass::GetID() const \
    {\
        return &ServiceClass::ms_RTTI; \
    }\
    XCr::String ServiceClass::GetServiceName() const \
    {\
        return ServiceClass::GetName(); \
    }\
    XLN_IMPLEMENT_RTTI(ServiceClass, baseclassname)
    
#define XLN_DECLARE_ROOT_SERVICE_CLASS(ServiceClass) \
    private: \
public: \
    static XCr::String GetName() {return ms_RTTI.GetName();}; \
    virtual ServiceID GetID() const; \
    virtual XCr::String GetServiceName() const; \
    XLN_DECLARE_ROOT_RTTI(ServiceClass);
    
#define XLN_IMPL_ROOT_SERVICE_CLASS(ServiceClass) \
    XCr::Service::ServiceID ServiceClass::GetID() const \
    {\
    return &ServiceClass::ms_RTTI; \
    }\
    XCr::String ServiceClass::GetServiceName() const \
    {\
    return ServiceClass::GetName(); \
    }\
    XLN_IMPLEMENT_ROOT_RTTI(ServiceClass)

//#include <boost/uuid/uuid_generators.hpp>
/*
boost::uuids::uuid dns_namespace_uuid(boost::uuids::nil_uuid()); \
boost::uuids::name_generator s_NameGenerator(dns_namespace_uuid);\
return s_NameGenerator(ServiceClass::GetName().c_str());\
*/

#define XLN_DECLARE_CREATOR(class) \
public: \
    static class* Create(); \
private: \
    static CreatorInitializer<class> s_Initializer; \


#define XLN_IMPLEMENT_CREATOR(class) \
    class* class::Create() \
    { \
        return XLN_OBJ_NEW class(); \
    } \
    XCr::Service::CreatorInitializer<class> class::s_Initializer; \

namespace XCr
{

class ServiceMgr;



class XLN_CLASS_DECL Service : public MemObj
{   public:
        typedef const XCr::RTTI* ServiceID;
        
        //XLN_DECLARE_RTTI(Service);
        //XLN_DECLARE_SERVICE_CLASS(Service);
        XLN_DECLARE_ROOT_SERVICE_CLASS(Service);
    
    public:
    
        
        //static boost::uuids::name_generator s_NameGenerator;
        
        Service();
        virtual ~Service(void);    
    
        virtual void OnPreInit() {};
        virtual bool OnInit() {return true;};
        virtual void OnRender() {};
        virtual bool OnTick(float delta) {return true;};
        virtual bool OnDestroy() {return true;};
        virtual void GetDependency(std::vector<ServiceID>& dependencies) {};
        
        //virtual std::string GetName() {return "Service"};
        
        void SetServiceMgr(XCr::ServiceMgr* serviceMgr);
        XCr::ServiceMgr* GetServiceMgr();
        
        template<class T>
        static ServiceID GetServiceID()
        {
            //boost::uuids::uuid dns_namespace_uuid(boost::uuids::nil_uuid());
            
            //boost::uuids::name_generator s_NameGenerator(dns_namespace_uuid);
            return &T::ms_RTTI;
            //s_NameGenerator(T::GetName().c_str());//(ServiceID)s_NameGenerator(GetName())//(&typeid(T));
        }

        static std::map<XCr::Service::ServiceID, folly::Function<Service*(void)>>& GetCreators();

        
        //ServiceID GetID();
        
        
    protected:

    template<class T>
    class CreatorInitializer
    {
        public:
            CreatorInitializer()
            {
                //int s = XCr::Service::s_Creators.size();
                //XLN_LOGE("%d", s);
                GetCreators().insert(std::make_pair(&T::ms_RTTI, T::Create));
            }
    };

        XCr::ServiceMgr* m_ServiceMgr;



};

}
