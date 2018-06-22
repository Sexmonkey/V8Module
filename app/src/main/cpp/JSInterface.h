//
// Created by JerryZhu on 2018/6/22.
//

#ifndef V8MODULE_BIND_H
#define V8MODULE_BIND_H

#include "v8.h"
using namespace v8;
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

class JSInterface {

public:
    static  void bind(v8pp::module module,Isolate*);
};


#endif //V8MODULE_BIND_H
