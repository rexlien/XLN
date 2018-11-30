//
// Created by Rex Lien on 2/16/17.
//

#include "PlatformServiceAndroid.h"
#include <Platform/android/src/main/cpp/Jni.h>
#include <Platform/android/src/main/cpp/JniMethodType.h>
#include <Platform/android/src/main/cpp/JniSignature.h>
#include <Platform/android/src/main/cpp/JniObject.h>
#include <XLN/Core/Foundation/Log.h>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <cpu-features.h>
#include <sys/system_properties.h>
extern "C"
{
#include "avcall.h"
#include "stdarg.h"
}


using namespace XCr;

XLN_IMPL_SERVICE_CLASS(PlatformServiceAndroid, XCr::PlatformService);

PlatformServiceAndroid::PlatformServiceAndroid() {

}

PlatformServiceAndroid::~PlatformServiceAndroid() {

}



bool PlatformServiceAndroid::OnInit()
{
    JNIEnv* env = JniHelper::GetEnv();

    jclass EnvClass = env->FindClass("android/os/Environment");
    jmethodID getExternalStorageDir = env->GetStaticMethodID(EnvClass, "getExternalStorageDirectory", "()Ljava/io/File;");
    jobject externalStoragePath = env->CallStaticObjectMethod(EnvClass, getExternalStorageDir, nullptr);
    jmethodID getFilePath = env->GetMethodID(env->FindClass("java/io/File"), "getPath", "()Ljava/lang/String;");
    jstring pathString = (jstring)env->CallObjectMethod(externalStoragePath, getFilePath, nullptr);
    const char *nativePathString = env->GetStringUTFChars(pathString, 0);
    m_WritableRoot = nativePathString;
    m_WritableRoot += "/";
    env->ReleaseStringUTFChars(pathString, nativePathString);


    
    return PlatformService::OnInit();
}


XCr::String XCr::PlatformServiceAndroid::GetWritableRootPath() const
{
    // Copy that somewhere safe
    //GFilePathBase = FString(nativePathString);

    /*
    JniMethodInfo t;
    jstring str;
    if(JniHelper::GetStaticMethodInfo(t, "com/lgengine/util/FileUtil", "getWriteFilePath", "()Ljava/lang/String;"))
    {
        str = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID);
        t.env->DeleteLocalRef(t.classID);
        return JniHelper::JavaString2String(str);;
    }
*/
    return m_WritableRoot;
}


int PlatformServiceAndroid::GetCPUCount() {
    return android_getCpuCount();
}

XCr::String PlatformServiceAndroid::GetOSName() {

    char osVersion[PROP_VALUE_MAX+1];
    __system_property_get("ro.build.version.release", osVersion);
    return XCr::String("Android") + XCr::String(osVersion);

}

XCr::String PlatformServiceAndroid::GetDeviceModel() {

    char model_string[PROP_VALUE_MAX+1];
    __system_property_get("ro.product.model", model_string);
    return XCr::String(model_string);

}

XCr::String PlatformServiceAndroid::GetCarrierName() {

    return CallPlatformMethod<XCr::String>("com/lgengine/Platform", "getCarrierName");
}

XCr::String PlatformServiceAndroid::GetNetworkType() {
    return CallPlatformMethod<XCr::String>("com/lgengine/Platform", "getNetworkType");
}


static void VFunctionVoid(JniMethodInfo* t, ...)
{
    va_list args;
    va_start(args, t);
    t->env->CallStaticVoidMethodV(t->classID, t->methodID, args);
    va_end(args);
}

static jobject VFunctionObject(JniMethodInfo* t, ...)
{
    va_list args;
    va_start(args, t);
    jobject obj = t->env->CallStaticObjectMethodV(t->classID, t->methodID, args);
    va_end(args);
    return obj;
}



template<>
void PlatformService::GetSignatureValueOne<XCr::String>( XCr::String& signature, folly::dynamic& object, const::XCr::String& arg)
{
    XCr::jni::TypeSignature<XCr::jni::String> sig;
    object.push_back(arg);
    signature += sig();
}

void PlatformServiceAndroid::GetSignatureBegin(XCr::String& signature)
{
    signature += "(";
}

void PlatformServiceAndroid::GetSignatureEnd(XCr::String& signature)
{
    signature += ")";
}

template<>
void PlatformService::GetSignatureValueReturn<void>(XCr::String& signature, folly::dynamic& ret)
{
    XCr::jni::TypeSignature<void> sig;
    signature += sig();
    ret = nullptr;
}

template<>
void PlatformService::GetSignatureValueReturn<XCr::String>(XCr::String& signature, folly::dynamic& ret)
{
    XCr::jni::TypeSignature<XCr::jni::String> sig;
    signature += sig();
    ret = "";
}
/*
void PlatformServiceAndroid::CallPlatformMethod(const XCr::String& className, const XCr::String& method,  const folly::dynamic& object)
{
    JniMethodInfo t;

    XCr::jni::TypeSignature<void (XCr::jni::String, XCr::jni::String)> signature;
    std::list<jobject> recycleList;

    if(JniHelper::GetStaticMethodInfo(t, className.c_str(), method.c_str(), signature()))//"(Ljava/lang/String;Ljava/lang/String;)V"))
    {
        av_alist alist;
        av_start_void(alist,VFunction );
        av_ptr(alist, JniMethodInfo_*, &t);

        for (auto& val : object)
        {
            if(val.isString())
            {
                jstring jStr = t.env->NewStringUTF(val.getString().c_str());
                av_ptr(alist, jstring, jStr);
                recycleList.push_back(jStr);
            }
            else if(val.isInt())
            {
                jint jInt = val.getInt();
                av_ptr(alist, jint, jInt);
            }
        }


        //decltype(&callFunction<jstring, jstring>) p = &callFunctionh<jstring, jstring>;

        av_call(alist);

        //((t.env)->*methodPtrf)(t.classID, t.methodID, javaWebViewID, javaScript);

        for(auto& val : recycleList)
        {
            t.env->DeleteLocalRef(val);
        }
        t.env->DeleteLocalRef(t.classID);

    }
}
*/
void PlatformServiceAndroid::CallPlatformMethodImpl(const XCr::String& className, const XCr::String& method, const XCr::String& signature,  folly::dynamic& ret, const folly::dynamic& object)
{

    XCr::String jClassName = className;
    boost::replace_all(jClassName, "::", "/");

    std::string jMethodName = method.substr(0, method.find(":", 0));
    JniMethodInfo t;
    std::list<jobject> recycleList;

    if(JniHelper::GetStaticMethodInfo(t, jClassName.c_str(), jMethodName.c_str(), signature.c_str()))//"(Ljava/lang/String;Ljava/lang/String;)V"))
    {
        av_alist alist;

        void* start_ret = 0;
        if(ret.isNull())
        {
            av_start_void(alist, VFunctionVoid);
        }
        else if(ret.isString())
        {
            jobject jobj;
            av_start_ptr(alist, VFunctionObject, jobject, &start_ret);
        }

        av_ptr(alist, JniMethodInfo_*, &t);

        for (auto& val : object)
        {
            if(val.isString())
            {
                jstring jStr = t.env->NewStringUTF(val.getString().c_str());
                av_ptr(alist, jstring, jStr);
                recycleList.push_back(jStr);
            }
            else if(val.isInt())
            {
                jint jInt = val.getInt();
                av_ptr(alist, jint, jInt);
            }
        }

        av_call(alist);
        if(start_ret)
        {
            if(ret.isString())
            {
                jstring str = (jstring)start_ret;
                XCr::String cStr = JniHelper::JavaString2String(str);
                ret = cStr.c_str();

            }
        }

        for(auto& val : recycleList)
        {
            t.env->DeleteLocalRef(val);
        }
        t.env->DeleteLocalRef(t.classID);

    }
}



