//
// Created by JerryZhu on 2018/5/25.
//

#include <sys/types.h>
#include <stdlib.h>
#include <android/asset_manager_jni.h>
#include <android/asset_manager.h>
#include <assert.h>
#include <class.hpp>
#include <module.hpp>
#include "include/libplatform/libplatform.h"
#include "v8Helper.h"
#include "log.h"
#include "LocalCFunction.h"
#include "Person.h"

static JNIEnv *jniEnv;
static jobject assetsmanager;

const char* v8Helper::loadScriptSource(const char *name)
{
    LOGI("loadScriptSource.................");
    std::string source("script/");
    std::string str2(".js");
    source.append(name).append(str2);
    AAssetManager* mgr = AAssetManager_fromJava(jniEnv, assetsmanager);
    if(mgr==NULL)
    {
        LOGI(" %s","AAssetManager==NULL");
        return 0;
    }
    AAsset* asset = AAssetManager_open(mgr,source.c_str(),AASSET_MODE_UNKNOWN);
    if(asset==NULL)
    {
        return 0;
    }
    off_t bufferSize = AAsset_getLength(asset);
    char *buffer=(char *)malloc(bufferSize+1);
    buffer[bufferSize]=0;
    AAsset_read(asset, buffer, bufferSize);
//    free(buffer);
    AAsset_close(asset);
    return buffer;
}

void v8Helper::setEnvAndAessets(JNIEnv *env, jobject assets) {
    jniEnv = env;
    assetsmanager = assets;
}
Local<String> v8Helper::toV8String(Isolate *isolate, const char * str)
{
    return String::NewFromUtf8(isolate, str,
                        NewStringType::kNormal).ToLocalChecked();
}


namespace  v8::internal
{
    void ReadNatives(){}
    void DisposeNatives(){}
    void SetNativesFromFile(v8::StartupData *s){}
    void SetSnapshotFromFile(v8::StartupData *s){}
}











