//
// Created by Rex Lien on 2/16/17.
//

#pragma once

#include <XLN/Core/Config/Config.h>
#include <XLN/Core/Foundation/Service.h>
#include <XLN/Core/Platform/PlatformService.h>

namespace XCr
{

class XLN_CLASS_DECL PlatformServiceAndroid : public XCr::PlatformService
{
    XLN_DECLARE_SERVICE_CLASS(PlatformServiceAndroid)


public:
        PlatformServiceAndroid();
        virtual ~PlatformServiceAndroid();

        virtual bool OnInit() override;
        virtual XCr::String GetWritableRootPath() const;
        //virtual bool IsPathExist(const XCr::String &path) const override;
        //virtual bool MakeDirectory(const XCr::String &path) override;

        virtual int GetCPUCount() override;
        virtual XCr::String GetOSName() override;
        virtual XCr::String GetDeviceModel() override;
        virtual XCr::String GetCarrierName() override;
        virtual XCr::String GetNetworkType() override;


    //void InjectJScript(const XCr::String &webviewID, const XCr::String &jScript);
        //virtual void CallPlatformMethod(const XCr::String& className, const XCr::String& method, const folly::dynamic& object);

protected:
        virtual void GetSignatureBegin(XCr::String& signature) override;
        virtual void GetSignatureEnd(XCr::String& signature) override;
        virtual void CallPlatformMethodImpl(const XCr::String& className, const XCr::String& method, const XCr::String& signature, folly::dynamic& ret, const folly::dynamic& object) override;

private:
        XCr::String m_WritableRoot;
};

}

