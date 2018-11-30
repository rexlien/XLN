//
// Created by Rex Lien on 1/25/17.
//

#include "Jni.h"
#include <android/log.h>
#include <Foundation/ServiceMgr.h>
#include <Framework/ExecutorService.h>
#include <Framework/Application.h>
//#include <Framework/Message/WebViewMessage.h>

using namespace XCr;


JavaVM* JniHelper::_psJavaVM = nullptr;

pthread_key_t JniHelper::s_Key;

jmethodID JniHelper::s_LoadclassMethod_methodID = nullptr;
jobject JniHelper::s_Classloader = nullptr;

XCr::String JniHelper::JavaString2String(jstring str)
{
    JNIEnv * jenv = JniHelper::GetEnv();

    const char * utf8string = jenv->GetStringUTFChars(str, 0);
    XCr:String result(utf8string);
    jenv->ReleaseStringUTFChars(str, utf8string);
    return result;
}

extern "C"
{
/*
JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved)
{

    //__android_log_print(ANDROID_LOG_FATAL, "app", "Jni_OnLoad");
    JniHelper::SetJavaVM(vm);
    JniHelper::SetClassLoader();

    return JNI_VERSION_1_4;
}
*/

//JNIEXPORT void Java_xln_core_Jni_PostExecutorService(JNIEnv *env, jobject /* this */, jstring src, jstring message)
//{
//    XCr::String cppSrc = JniHelper::JavaString2String(src);
//    XCr::String cppMsg = JniHelper::JavaString2String(message);
//
//    XGf::ExecutorService* execService = XCr::ServiceMgr::GetActiveServiceMgr()->GetService<XGf::ExecutorService>();
//    XCr::SharedPtr<XGf::WebViewMessage> webviewMsg = std::make_shared<XGf::WebViewMessage>(cppSrc, XGf::WebViewMessage::Command::LOAD_URL);
//    webviewMsg->InsertValue("url", cppMsg);
//
//    execService->PostRenderFrame(Task::Category::CATEGORY_ALL, std::static_pointer_cast<XCr::IMessage>(webviewMsg));
//
//}


}

