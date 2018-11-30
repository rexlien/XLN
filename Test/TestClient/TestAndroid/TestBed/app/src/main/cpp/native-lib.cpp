#include <jni.h>
#include <string>

#include <XLN/Core/Foundation/Log.h>

extern "C"
jstring
Java_com_example_rexlien_testbed_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++12";
    return env->NewStringUTF(hello.c_str());
}
