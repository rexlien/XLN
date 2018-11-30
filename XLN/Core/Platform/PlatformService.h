#pragma once

#include <XLN/Core/Config/Config.h>
#include <XLN/Core/Foundation/Service.h>
#include <folly/dynamic.h>
#include <folly/DynamicConverter.h>

namespace XCr
{
    
    class XLN_CLASS_DECL PlatformService : public XCr::Service
    {
        XLN_DECLARE_SERVICE_CLASS(PlatformService);
        public:
        
            PlatformService();
            virtual ~PlatformService()
            {
            
            }

            virtual bool OnInit() override;

            virtual XCr::String GetWritableRootPath() const = 0;
            virtual XCr::String GetXLNRoot() const;
            virtual XCr::String GetAppRoot() const;

            bool IsPathExist(const XCr::String& path) const;
            bool MakeDirectory(const XCr::String& path, bool clearOnExist = false);

            //virtual void InjectJScript(const XCr::String& webviewID, const XCr::String& jScript) = 0;
            //virtual void CallPlatformMethod(const XCr::String& className, const XCr::String& method, const folly::dynamic& object) = 0;

        //template<typename... T>
        //void CallPlatformMethods(const XCr::String& className, const XCr::String& method, T... args) = 0;


            template <typename ...T>
            void GetSignatureValue(  XCr::String& signature, folly::dynamic& object, T&... t)
            {
                //GetSignatureValueOne(signature, object, arg);
                //GetSignatureValue(signature, object, t...);
            }

        template <typename H, typename ...T>
            void GetSignatureValue(  XCr::String& signature, folly::dynamic& object, H arg, T&... t)
            {
                GetSignatureValueOne(signature, object, arg);
                GetSignatureValue(signature, object, t...);
            }

            template <typename H>
            void GetSignatureValue( XCr::String& signature, folly::dynamic& object, H arg)
            {
                GetSignatureValueOne(signature, object, arg);

            }

            template <typename H>
            void GetSignatureValueOne( XCr::String& signature, folly::dynamic& object, const H& arg);

            template <typename R>
            void GetSignatureValueReturn( XCr::String& signature, folly::dynamic& ret);

            template<typename R, typename ...T>
            R CallPlatformMethod(const XCr::String& className, const XCr::String& method, const T&... args)
            {
                XCr::String sig;
                folly::dynamic arr = folly::dynamic::array;
                folly::dynamic ret = nullptr;
                GetSignatureBegin(sig);
                GetSignatureValue(sig, arr, args...);
                GetSignatureEnd(sig);
                GetSignatureValueReturn<R>(sig, ret);

                CallPlatformMethodImpl(className, method, sig, ret, arr);

                return folly::convertTo<R>(ret);

            };

            template<typename ...T>
            void CallPlatformMethodVoid(const XCr::String& className, const XCr::String& method, const T&... args)
            {
                XCr::String sig;
                folly::dynamic arr = folly::dynamic::array;
                folly::dynamic ret = nullptr;
                GetSignatureBegin(sig);
                GetSignatureValue(sig, arr, args...);
                GetSignatureEnd(sig);
                GetSignatureValueReturn<void>(sig, ret);
                CallPlatformMethodImpl(className, method, sig, ret, arr);

            };

            virtual int GetCPUCount();
            virtual XCr::String GetOSName();
            virtual XCr::String GetDeviceModel();
            virtual XCr::String GetCarrierName();
            virtual XCr::String GetNetworkType();

        protected:
            virtual void GetSignatureBegin(XCr::String& signature) {};
            virtual void GetSignatureEnd(XCr::String& signature) {};
            virtual void CallPlatformMethodImpl(const XCr::String& className, const XCr::String& method, const XCr::String& signature, folly::dynamic& ret, const folly::dynamic& object) = 0;

            XCr::String m_XLNRoot;
            XCr::String m_AppRoot;
        
    };

    
}