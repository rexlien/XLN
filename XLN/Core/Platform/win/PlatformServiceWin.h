#pragma once

#include <XLN/Core/Config/Config.h>
#include <XLN/Core/Foundation/Service.h>
#include <XLN/Core/Platform/PlatformService.h>


namespace XCr
{
    class XLN_CLASS_DECL PlatformServiceWin : public XCr::PlatformService
    {
        XLN_DECLARE_SERVICE_CLASS(PlatformServiceWin)

        public:
            virtual XCr::String GetWritableRootPath() const;
            virtual void CallPlatformMethodImpl(const XCr::String& className, const XCr::String& method, const XCr::String& signature, folly::dynamic& ret, const folly::dynamic& object);
    };

}