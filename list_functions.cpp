#include "list_functions.h"

#include "error.h"
#include "gc.h"

Object* IsPair::Apply(Object* obj) const {
    Params params;
    GetEvaluatedParams(obj, params);
    if (params.size() != 1) {
        throw RuntimeError("Wrong number of params");
    }
    if (const auto p = As<Cell>(params[0])) {
        params.clear();
        GetParams(p, params);
        return Heap::GetInstance().Make<Boolean>(params.size() == 2);
    }
    return Heap::GetInstance().Make<Boolean>(false);
}

Object* IsNull::Apply(Object* obj) const {
    Params params;
    GetEvaluatedParams(obj, params);
    if (params.size() != 1) {
        throw RuntimeError("Wrong number of params");
    }
    return Heap::GetInstance().Make<Boolean>(!params[0]);
}

Object* IsList::Apply(Object* obj) const {
    Params params;
    GetEvaluatedParams(obj, params);
    if (params.size() != 1) {
        throw RuntimeError("Wrong number of params");
    }
    auto for_check = params[0];
    while (Is<Cell>(for_check)) {
        const auto p = As<Cell>(for_check);
        for_check = p->GetSecond();
    }
    return Heap::GetInstance().Make<Boolean>(!for_check);
}

Object* Cons::Apply(Object* obj) const {
    Params params;
    GetEvaluatedParams(obj, params);
    if (params.size() != 2) {
        throw RuntimeError("Wrong number of params");
    }
    return Heap::GetInstance().Make<Cell>(params[0], params[1]);
}

Object* Car::Apply(Object* obj) const {
    Params params;
    GetEvaluatedParams(obj, params);
    if (params.size() != 1) {
        throw RuntimeError("Wrong number of params");
    }
    if (const auto p = As<Cell>(params[0])) {
        return p->GetFirst();
    }
    throw RuntimeError("Expected a list param");
}

Object* Cdr::Apply(Object* obj) const {
    Params params;
    GetEvaluatedParams(obj, params);
    if (params.size() != 1) {
        throw RuntimeError("Wrong number of params");
    }
    if (const auto p = As<Cell>(params[0])) {
        return p->GetSecond();
    }
    throw RuntimeError("Expected a list param");
}

Object* List::Apply(Object* obj) const {
    return obj;
}

Object* ListRef::Apply(Object* obj) const {
    Params params;
    GetEvaluatedParams(obj, params);
    if (params.size() != 2) {
        throw RuntimeError("Wrong number of params");
    }
    if (!Is<Number>(params[1]) || !Is<Cell>(params[0])) {
        throw RuntimeError("Wrong types of params");
    }
    int64_t index = As<Number>(params[1])->GetValue();
    const auto p = As<Cell>(params[0]);
    params.clear();
    GetParams(p, params);
    if (index < 0 || index >= params.size()) {
        throw RuntimeError("Wrong index");
    }
    return params[index];
}

Object* ListTail::Apply(Object* obj) const {
    Params params;
    GetEvaluatedParams(obj, params);
    if (params.size() != 2) {
        throw RuntimeError("Wrong number of params");
    }
    if (!Is<Number>(params[1]) || !Is<Cell>(params[0])) {
        throw RuntimeError("Wrong types of params");
    }
    int64_t index = As<Number>(params[1])->GetValue();
    auto p = As<Cell>(params[0]);
    if (index < 0) {
        throw RuntimeError("Wrong index");
    }
    for (size_t i = 0; i < index; ++i) {
        if (!p) {
            throw RuntimeError("Wrong index");
        }
        p = As<Cell>(p->GetSecond());
    }
    return p;
}
