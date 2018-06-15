//
// Created by JerryZhu on 2018/6/13.
//

#include <string>
#include <cassert>
#include "LocalCFunction.h"
#include "log.h"
#include "v8Helper.h"

 void LocalCFunction::print(const FunctionCallbackInfo<Value> &args)
 {
     bool first = true;
     for (int i = 0; i < args.Length(); i++) {
         HandleScope handle_scope(args.GetIsolate());
         if (first) {
             first = false;
         } else {
             printf(" 1111");
         }
         String::Utf8Value str(args.GetIsolate(),args[i]);
         LOGI("print is called : %s",*str);
         std::string result(*str);
     }

     fflush(stdout);

 }
void LocalCFunction::add(const FunctionCallbackInfo<Value> &args)
{
    int a = args[0]->Uint32Value();
    int b = args[1]->Uint32Value();
    args.GetReturnValue().Set(
            Integer::New(args.GetIsolate(), a+b));
    LOGI("add is called");
}

Local<Value> ExecuteScript1(Isolate *isolate, const char *source, bool print_result,
                                           bool report_exceptions)
{
//    LOGI("execute js source ....%s: \n",source);
    EscapableHandleScope  handle_scope(isolate);

    TryCatch try_catch(isolate);

    Local<Context> context(isolate->GetCurrentContext());

    Local<String> v8_source =v8Helper::toV8String(isolate,source);

    Local<Script> script;
    if (!Script::Compile(context,v8_source).ToLocal(&script)) {
        // Print errors that happened during compilation.
        if (report_exceptions)
            LocalCFunction::ReportException(isolate, &try_catch);
        return Undefined(isolate);
    } else {
        Local<Value> result;
        if (!script->Run(context).ToLocal(&result)) {
            assert(try_catch.HasCaught());
            // Print errors that happened during execution.
            if (report_exceptions)
                LocalCFunction::ReportException(isolate, &try_catch);
            return Undefined(isolate);
        } else {
            assert(!try_catch.HasCaught());
            if (print_result && !result->IsUndefined()) {
                // If all went well and the result wasn't undefined then print
                // the returned value.
                String::Utf8Value str(isolate, result);
                LOGI("execute js source result %s\n", *str);
            }

            return handle_scope.Escape(result);
        }
    }
}

void LocalCFunction::ExecuteScript2(const FunctionCallbackInfo<Value> &args)
{

    Isolate *isolate = args.GetIsolate();
    EscapableHandleScope scope(isolate);
    String::Utf8Value source(isolate, args[0]);

//    LOGI("ExecuteScript args %s",*source);

    Local<Value> process_fun = ExecuteScript1(isolate,*source,true,true);
    args.GetReturnValue().Set(scope.Escape(process_fun));

}
Local<Value> LocalCFunction::ExecuteScript(Isolate *isolate, const char *name, bool print_result,
                                     bool report_exceptions)
{
    const char* source = v8Helper::loadScriptSource(name);

//    LOGI("execute js source ....%s: \n",source);
    EscapableHandleScope  handle_scope(isolate);
    TryCatch try_catch(isolate);
    Local<Context> context(isolate->GetCurrentContext());

    Local<String> v8_source =v8Helper::toV8String(isolate,source);

    Local<Script> script;
    if (!Script::Compile(context,v8_source).ToLocal(&script)) {
        // Print errors that happened during compilation.
        if (report_exceptions)
            ReportException(isolate, &try_catch);
        return Undefined(isolate);
    } else {
        Local<Value> result;
        if (!script->Run(context).ToLocal(&result)) {
            assert(try_catch.HasCaught());
            // Print errors that happened during execution.
            if (report_exceptions)
                ReportException(isolate, &try_catch);
            return Undefined(isolate);
        } else {
            assert(!try_catch.HasCaught());
            if (print_result && !result->IsUndefined()) {
                // If all went well and the result wasn't undefined then print
                // the returned value.
                String::Utf8Value str(isolate, result);
                LOGI("execute js source result %s\n", *str);
            }
            //调用js方法
            Local<String> function_name =
                    String::NewFromUtf8(isolate, "runMain", NewStringType::kNormal)
                            .ToLocalChecked();
            Local<Value> jsFunction_val;
            // If there is no Process function, or if it is not a function,
            if (!context->Global()->Get(context, function_name).ToLocal(&jsFunction_val) ||
                !jsFunction_val->IsFunction()) {
                LOGI("jsFunction is not a function\n");
            }
            // It is a function; cast it to a Function
            Local<Function> jsFunction_fun = Local<Function>::Cast(jsFunction_val);
            const int argc = 0;
            Local<Value> argv[argc] = {};
            Local<Value> call_result;
            if (!jsFunction_fun -> Call(context, context->Global(), argc, argv).ToLocal(&call_result)) {
                String::Utf8Value error(isolate, try_catch.Exception());
                LOGI("call js function error:%s",*error);
            }
            return handle_scope.Escape(result);
        }
    }
}
void LocalCFunction::ReportException(Isolate *isolate, TryCatch *try_catch)
{
    HandleScope handle_scope(isolate);
    String::Utf8Value exception(isolate, try_catch->Exception());
    Local<Message> message = try_catch->Message();
    if (message.IsEmpty()) {
        // V8 didn't provide any extra information about this error; just
        // print the exception.
        LOGI("exception_string ; %s\n", *exception);
    } else {
        String::Utf8Value filename(isolate,
                                   message->GetScriptOrigin().ResourceName());
        Local<Context> context(isolate->GetCurrentContext());
        int linenum = message->GetLineNumber(context).FromJust();
        LOGI("exception_string : %s:%i: %s\n",  *filename, linenum, *exception);
        // Print line of source code.
        String::Utf8Value sourceline(
                isolate, message->GetSourceLine(context).ToLocalChecked());
        LOGI("stderr :%s\n",*sourceline);
        // Print wavy underline (GetUnderline is deprecated).
        int start = message->GetStartColumn(context).FromJust();
        for (int i = 0; i < start; i++) {
            fprintf(stderr, " ");
        }
        int end = message->GetEndColumn(context).FromJust();
        for (int i = start; i < end; i++) {
            fprintf(stderr, "^");
        }
        fprintf(stderr, "\n");
        Local<Value> stack_trace_string;
        if (try_catch->StackTrace(context).ToLocal(&stack_trace_string) &&
            stack_trace_string->IsString() &&
            Local<v8::String>::Cast(stack_trace_string)->Length() > 0) {
            String::Utf8Value stack_trace(isolate, stack_trace_string);
            LOGI("exception_string : %s\n\n",*stack_trace);
        }
    }

}
void LocalCFunction::loadScriptSource(const FunctionCallbackInfo<Value> &args)
{
    Isolate *isolate = args.GetIsolate();
    EscapableHandleScope scope(isolate);
    String::Utf8Value name(isolate, args[0]);

    Local<String> source = v8Helper::toV8String(isolate,v8Helper::loadScriptSource(*name));

    args.GetReturnValue().Set(scope.Escape(source));
}