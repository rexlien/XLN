//
// Created by Rex Lien on 1/25/17.
//

#pragma one

#include <XLN/Core/Config/Config.h>
#include <jni.h>
#include <pthread.h>
#include <XLN/Core/Foundation/Log.h>

namespace XCr {


typedef struct JniMethodInfo_
{
    JNIEnv *env;
    jclass classID;
    jmethodID methodID;
} JniMethodInfo;


class XLN_CLASS_DECL JniHelper
{
public:
    static JavaVM *_psJavaVM;
    static pthread_key_t s_Key;
    static jmethodID s_LoadclassMethod_methodID;
    static jobject s_Classloader;

    static void SetJavaVM(JavaVM *javaVM) {
        pthread_t thisthread = pthread_self();
        //LOGD("JniHelper::setJavaVM(%p), pthread_self() = %ld", javaVM, thisthread);
        _psJavaVM = javaVM;

        pthread_key_create(&s_Key, _detachCurrentThread);
    }

    static JavaVM *GetJavaVM() {
        return _psJavaVM;
    }

    static JNIEnv *cacheEnv(JavaVM *jvm) {
        JNIEnv *_env = nullptr;
        // get jni environment
        jint ret = jvm->GetEnv((void **) &_env, JNI_VERSION_1_4);

        switch (ret) {
            case JNI_OK :
                // Success!
                pthread_setspecific(s_Key, _env);
                return _env;

            case JNI_EDETACHED :
                // Thread not attached
                if (jvm->AttachCurrentThread(&_env, nullptr) < 0) {
                    XLN_LOGE("Failed to get the environment using AttachCurrentThread()");

                    return nullptr;
                } else {
                    // Success : Attached and obtained JNIEnv!
                    pthread_setspecific(s_Key, _env);
                    return _env;
                }

            case JNI_EVERSION :
                // Cannot recover from this error
                XLN_LOGE("JNI interface version 1.4 not supported");
            default :
                XLN_LOGE("Failed to get the environment using GetEnv()");
                return nullptr;
        }
    }

    static bool getMethodInfo_DefaultClassLoader(JniMethodInfo &methodinfo,
                                                 const char *className,
                                                 const char *methodName,
                                                 const char *paramCode) {
        if ((nullptr == className) ||
            (nullptr == methodName) ||
            (nullptr == paramCode)) {
            return false;
        }

        JNIEnv *env = JniHelper::GetEnv();
        if (!env) {
            return false;
        }

        jclass classID = env->FindClass(className);
        if (!classID) {
            XLN_LOGE("Failed to find class %s", className);
            env->ExceptionClear();
            return false;
        }

        jmethodID methodID = env->GetMethodID(classID, methodName, paramCode);
        if (!methodID) {
            XLN_LOGE("Failed to find method id of %s", methodName);
            env->ExceptionClear();
            return false;
        }

        methodinfo.classID = classID;
        methodinfo.env = env;
        methodinfo.methodID = methodID;

        return true;
    }

    static bool SetClassLoader() {
        JniMethodInfo _getclassloaderMethod;
        if (!JniHelper::getMethodInfo_DefaultClassLoader(_getclassloaderMethod,
                                                         "java/lang/Class",
                                                         "getClassLoader",
                                                         "()Ljava/lang/ClassLoader;")) {
            return false;
        }

        JNIEnv *env = GetEnv();
        jclass mainClass = env->FindClass("xln/core/XLNActivity");


        jobject _c = JniHelper::GetEnv()->CallObjectMethod(mainClass,
                                                           _getclassloaderMethod.methodID);

        if (nullptr == _c) {
            return false;
        }

        JniMethodInfo _m;
        if (!JniHelper::getMethodInfo_DefaultClassLoader(_m,
                                                         "java/lang/ClassLoader",
                                                         "loadClass",
                                                         "(Ljava/lang/String;)Ljava/lang/Class;")) {
            return false;
        }

        JniHelper::s_Classloader = JniHelper::GetEnv()->NewGlobalRef(_c);
        JniHelper::s_LoadclassMethod_methodID = _m.methodID;

        return true;
    }

    static jclass GetClassID(const char *className) {
        if (nullptr == className) {
            return nullptr;
        }

        JNIEnv *env = GetEnv();

        jstring _jstrClassName = env->NewStringUTF(className);

        jclass _clazz = (jclass) env->CallObjectMethod(JniHelper::s_Classloader,
                                                       JniHelper::s_LoadclassMethod_methodID,
                                                       _jstrClassName);

        if (nullptr == _clazz) {
            XLN_LOGE("Classloader failed to find class of %s", className);
            env->ExceptionClear();
        }

        env->DeleteLocalRef(_jstrClassName);

        return _clazz;
    }

    static JNIEnv *GetEnv() {
        JNIEnv *_env = (JNIEnv *) pthread_getspecific(s_Key);
        if (_env == nullptr)
            _env = JniHelper::cacheEnv(_psJavaVM);
        return _env;
    }

    static bool GetStaticMethodInfo(JniMethodInfo &methodinfo,
                                    const char *className,
                                    const char *methodName,
                                    const char *paramCode) {
        if ((nullptr == className) ||
            (nullptr == methodName) ||
            (nullptr == paramCode)) {
            return false;
        }

        JNIEnv *env = JniHelper::GetEnv();
        if (!env) {
            XLN_LOGE("Failed to get JNIEnv");
            return false;
        }

        jclass classID = GetClassID(className);
        if (!classID) {
            XLN_LOGE("Failed to find class %s", className);
            env->ExceptionClear();
            return false;
        }

        jmethodID methodID = env->GetStaticMethodID(classID, methodName, paramCode);
        if (!methodID) {
            XLN_LOGE("Failed to find static method id of %s", methodName);
            env->ExceptionClear();
            return false;
        }

        methodinfo.classID = classID;
        methodinfo.env = env;
        methodinfo.methodID = methodID;
        return true;
    }

    static void _detachCurrentThread(void *a) {
        _psJavaVM->DetachCurrentThread();
    }

    static XCr::String JavaString2String(jstring str);
};

}





