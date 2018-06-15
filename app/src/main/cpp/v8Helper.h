//
// Created by JerryZhu on 2018/5/25.
//

#ifndef V8DEMO_V8HELPER_H
#define V8DEMO_V8HELPER_H
#define OBJ_TEMPLATE(obj,isolate) Local<ObjectTemplate> obj = ObjectTemplate::New(isolate)
#define EXPOSE(obj,isolate, jsname, func) obj->Set(String::NewFromUtf8(isolate, jsname, NewStringType::kNormal).ToLocalChecked(),FunctionTemplate::New(isolate, func));

#include <string>
#include <jni.h>
#include "include/v8.h"

using namespace v8;

class v8Helper {

public:

    static const char* loadScriptSource(const char *name);
    static void setEnvAndAessets(JNIEnv *env, jobject Jobject);
    static Local<String> toV8String(Isolate* isolate, const char*);

};


#endif //V8DEMO_V8HELPER_H
