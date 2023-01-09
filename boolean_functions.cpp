#include "boolean_functions.h"

#include "error.h"
#include "gc.h"

Object* IsBoolean::Apply(Object* obj) const {
    Params params;
    GetEvaluatedParams(obj, params);
    if (params.size() != 1) {
        throw RuntimeError("Wrong number of params");
    }
    return Is<Boolean>(params[0]) ? Heap::GetInstance().Make<Boolean>(true)
                                  : Heap::GetInstance().Make<Boolean>(false);
}

Object* Not::Apply(Object* obj) const {
    Params params;
    GetEvaluatedParams(obj, params);
    if (params.size() != 1) {
        throw RuntimeError("Wrong number of params");
    }
    return Heap::GetInstance().Make<Boolean>(!IsTrue(params[0]));
}

Object* And::Apply(Object* obj) const {
    Params params;
    GetParams(obj, params);
    if (params.empty()) {
        return Heap::GetInstance().Make<Boolean>(true);
    }
    for (auto& i : params) {
        i = ApplyFunctions(i, scope_);
        if (!IsTrue(i)) {
            return i;
        }
    }
    return params.back();
}

Object* Or::Apply(Object* obj) const {
    Params params;
    GetParams(obj, params);
    if (params.empty()) {
        return Heap::GetInstance().Make<Boolean>(false);
    }
    for (auto& i : params) {
        i = ApplyFunctions(i, scope_);
        if (IsTrue(i)) {
            return i;
        }
    }
    return params.back();
}
