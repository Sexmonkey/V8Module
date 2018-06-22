//
// Created by JerryZhu on 2018/6/22.
//
#include "JSInterface.h"
void JSInterface::bind(v8pp::module module, Isolate * isolate)
{
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

    // set class into the module template
    module.set("Person", Person_class);
    //bind  static variables
    module.set("account", Person::account);

    module.set("print",&LocalCFunction::print);
    module.set("add",&LocalCFunction::add);
    module.set("compileScript",&LocalCFunction::compileScript);
    module.set("loadScriptSource",&LocalCFunction::loadScriptSource);
    isolate->GetCurrentContext()->Global()->Set(
            v8::String::NewFromUtf8(isolate, "NativeDevice"), module.new_instance());
}