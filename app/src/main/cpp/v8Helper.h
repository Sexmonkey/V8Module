//
// Created by JerryZhu on 2018/5/25.
//

#ifndef V8DEMO_V8HELPER_H
#define V8DEMO_V8HELPER_H

#include <string>
#include <jni.h>
#include "include/v8.h"

using namespace v8;

class v8Helper {

public:

    static const char* loadScriptSource(const char *name);
    static void setEnvAndAssets(JNIEnv *env, jobject Jobject);
    static Local<String> toV8String(Isolate* isolate, const char*);

};


#endif //V8DEMO_V8HELPER_H
