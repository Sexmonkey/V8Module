#include <jni.h>
#include <string>
#include <iostream>
#include "JSInterface.h"

extern "C"
JNIEXPORT jstring JNICALL
Java_com_boyaa_v8pp_1test_MainActivity_process(JNIEnv *env, jobject instance, jobject assets) {
    v8Helper::setEnvAndAssets(env,assets);
    std::unique_ptr<v8::Platform> platform(v8::platform::CreateDefaultPlatform());
    v8::V8::InitializePlatform(platform.get());
    v8::V8::Initialize();
    v8pp::context context;
    Isolate *isolate = context.isolate();
    v8::HandleScope scope(isolate);
    //create module
    v8pp::module NativeDevice(isolate);
    JSInterface::bind(NativeDevice,isolate);
    context.run_script(isolate,"module",true,true);

    return env->NewStringUTF("hello");

}