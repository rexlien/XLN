#include "ReactService.h"
#include <XLN/Core/Framework/ExecutorService.h>
#include <XLN/Core/Foundation/ServiceMgr.h>
#include <XLN/Core/Framework/RenderFrame.h>
#include <XLN/Core/Platform/PlatformService.h>

using namespace XGf;

XLN_IMPL_SERVICE_CLASS(XGf::ReactService, XCr::Service);

void XGf::ReactService::OnHandleMessage(XCr::SharedPtr<XCr::IMessage> message) {
    if(message->ObjClassID() == XGf::WebViewMessage::ClassID()) {

        XCr::SharedPtr<XGf::WebViewMessage> webViewMsg = std::static_pointer_cast<XGf::WebViewMessage>(message);

        XGf::WebViewMessage::Command command = webViewMsg->GetCommand();
        if(command == XGf::WebViewMessage::INJECT_JSCRIPT) {
            const XCr::String &id = webViewMsg->GetWebViewID();
            const XCr::String *script = webViewMsg->GetValue("script");
            if(script)
            {   XCr::PlatformService* platformService = XCr::ServiceMgr::GetActiveServiceMgr()->GetService<XCr::PlatformService>();

                //folly::dynamic object = folly::dynamic::object;
                folly::dynamic array = folly::dynamic::array(id, *script);

                //platformService->CallPlatformMethod("xln/react/WebViewManager", "callJavaScript", array);
                platformService->CallPlatformMethodVoid<XCr::String, XCr::String>("xln::react::WebViewManager",
                                                                                                  "callJavaScript:script:", id, *script);
            }

        }
    }
}
void ReactService::GetDependency(std::vector<ServiceID> &dependencies)
{
    dependencies.push_back(XCr::Service::GetServiceID<ExecutorService>());
}


bool ReactService::OnInit()
{
    XGf::ExecutorService* executorService = XCr::ServiceMgr::GetActiveServiceMgr()->GetService<XGf::ExecutorService>();
    if(executorService)
    {
        m_MessageHandler = executorService->GetRenderFrame()->AcquireEventHandler<XCr::MessageHandlerHelper<ReactService, XGf::WebViewMessage>>(this);
    }

    return true;
}

#if (XLN_PLATFORM == XLN_PLATFORM_ANDROID)
#include <jni.h>
#include <XLN/Core/Platform/android/src/main/cpp/Jni.h>
extern "C"
{
JNIEXPORT void Java_xln_react_Jni_OnWebViewLoadUrl(JNIEnv *env, jobject /* this */, jstring src,
                                                     jstring message) {
    XCr::String cppSrc = XCr::JniHelper::JavaString2String(src);
    XCr::String cppMsg = XCr::JniHelper::JavaString2String(message);

    XGf::ExecutorService *execService = XCr::ServiceMgr::GetActiveServiceMgr()->GetService<XGf::ExecutorService>();
    XCr::SharedPtr<XGf::WebViewMessage> webviewMsg = std::make_shared<XGf::WebViewMessage>(cppSrc,
                                                                                           XGf::WebViewMessage::Command::LOAD_URL);
    webviewMsg->InsertValue("url", cppMsg);

    execService->PostRenderFrame(XCr::Task::Category::CATEGORY_ALL,
                                 std::static_pointer_cast<XCr::IMessage>(webviewMsg));

}
}
#endif


