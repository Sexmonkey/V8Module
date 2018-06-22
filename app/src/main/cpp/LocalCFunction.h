//
// Created by JerryZhu on 2018/6/13.
//

#include "v8.h"
#ifndef V8PP_TEST_LOCALCFUNCTION_H
#define V8PP_TEST_LOCALCFUNCTION_H
using namespace v8;

class LocalCFunction {
public:
    static void print(const FunctionCallbackInfo <Value> &args);
    static void add(const FunctionCallbackInfo <Value> &args);
    static void loadScriptSource(const FunctionCallbackInfo <Value> &args);
    static void compileScript(const FunctionCallbackInfo <Value> &args);
    static void ReportException(Isolate* isolate, TryCatch* try_catch);
    static Local<Value> ExecuteScript(Isolate *isolate,const char *name, bool print_result,
                                      bool report_exceptions);
};


#endif //V8PP_TEST_LOCALCFUNCTION_H
