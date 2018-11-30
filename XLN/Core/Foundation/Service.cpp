#include "Service.h"


using namespace XCr;

//XLN_IMPL_SERVICE_CLASS(Service);
XLN_IMPL_ROOT_SERVICE_CLASS(Service);
//boost::uuids::name_generator Service::s_NameGenerator;
//std::map<XCr::Service::ServiceID, folly::Function<Service*(void)>> Service::s_Creators;

std::map<XCr::Service::ServiceID, folly::Function<Service*(void)>>& Service::GetCreators()
{
    static std::map<XCr::Service::ServiceID, folly::Function<Service*(void)>> s_Creators;
    return s_Creators;
}


Service::Service()
{
    m_ServiceMgr = 0;
}

Service::~Service(void)
{
    m_ServiceMgr = 0;
}

void XCr::Service::SetServiceMgr( XCr::ServiceMgr* serviceMgr )
{
    m_ServiceMgr = serviceMgr;
}

XCr::ServiceMgr* XCr::Service::GetServiceMgr()
{
    return m_ServiceMgr;
}

