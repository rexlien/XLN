#pragma once
#include <XLN/Core/Config/Config.h>
#include <XLN/Core/Foundation/Service.h>
#include <XLN/Core/Platform/PlatformService.h>

namespace XCr
{
    
    class XLN_CLASS_DECL PlatformServiceiOS : public XCr::PlatformService
    {
        XLN_DECLARE_SERVICE_CLASS(PlatformServiceiOS);
    public:
        
        PlatformServiceiOS();
        
        virtual ~PlatformServiceiOS();
        //virtual XCr::String GetWritableFileRoot() const;
        
        virtual XCr::String GetWritableRootPath() const;
        //virtual bool MakeDirectory(const XCr::String& path);
        
        virtual void CallPlatformMethodImpl(const XCr::String& className, const XCr::String& method, const XCr::String& signature, folly::dynamic& ret, const folly::dynamic& object);
        virtual int GetCPUCount() override;
        virtual XCr::String GetOSName() override;
        virtual XCr::String GetDeviceModel() override;
        virtual XCr::String GetCarrierName() override;
        virtual XCr::String GetNetworkType() override;
        
        
    };
    
    
}
