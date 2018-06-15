#include <jni.h>
#include <string>
#include <iostream>
#include "v8.h"
#include <algorithm>
#include <memory>
#include <vector>
#include <exception>
#include <libplatform/libplatform.h>
#include "v8pp/context.hpp"
#include "v8pp/module.hpp"
#include "v8pp/class.hpp"
#include "v8pp/property.hpp"
#include "v8Helper.h"
#include "Person.h"
#include "LocalCFunction.h"


extern "C"
JNIEXPORT jstring JNICALL
Java_com_boyaa_v8pp_1test_MainActivity_process(JNIEnv *env, jobject instance, jobject assets) {
    v8Helper::setEnvAndAessets(env,assets);
    std::unique_ptr<v8::Platform> platform(v8::platform::CreateDefaultPlatform());
    v8::V8::InitializePlatform(platform.get());
    v8::V8::Initialize();
    v8pp::context context;
    Isolate *isolate = context.isolate();
    v8::HandleScope scope(isolate);
    v8pp::module NativeDevice(isolate);
    // bind class
    v8pp::class_<Person> Person_class(isolate);
    Person_class
            // specify X constructor signature
            .ctor<std::string, int>()
                    // bind variable
            .set("name", &Person::name)

            .set("age", &Person::age)
                    // bind function
            .set("setName", &Person::setName)
                    // bind function
            .set("setAge", &Person::setAge)
                    // bind function
            .set("getAge", &Person::getAge)
                    // bind function
            .set("getName", &Person::getName);

//     // set class into the module template
    NativeDevice.set("Person", Person_class);

    NativeDevice.set("print",&LocalCFunction::print);
    NativeDevice.set("add",&LocalCFunction::add);
    NativeDevice.set("compileScript",&LocalCFunction::ExecuteScript2);
    NativeDevice.set("loadScriptSource",&LocalCFunction::loadScriptSource);
//       // set bindings in global object as `mylib`
    isolate->GetCurrentContext()->Global()->Set(
            v8::String::NewFromUtf8(isolate, "NativeDevice"), NativeDevice.new_instance());

    context.run_script(isolate,"module",true,true);

    return env->NewStringUTF("hello");

}