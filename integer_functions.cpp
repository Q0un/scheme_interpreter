#include "integer_functions.h"

#include "error.h"
#include "gc.h"

Object* IsNumber::Apply(Object* obj) const {
    Params params;
    GetEvaluatedParams(obj, params);
    if (params.size() != 1) {
        throw RuntimeError("Wrong number of params");
    }
    return Heap::GetInstance().Make<Boolean>(Is<Number>(params[0]));
}

Object* CompareInts::Apply(Object* obj, std::function<bool(int64_t, int64_t)>&& func) const {
    Params params;
    GetEvaluatedParams(obj, params);
    if (params.empty()) {
        return Heap::GetInstance().Make<Boolean>(true);
    }
    if (!Is<Number>(params[0])) {
        throw RuntimeError("Got not a number param");
    }
    for (size_t i = 1; i < params.size(); ++i) {
        if (!Is<Number>(params[i])) {
            throw RuntimeError("Got not a number param");
        }
        if (!func(As<Number>(params[i - 1])->GetValue(), As<Number>(params[i])->GetValue())) {
            return Heap::GetInstance().Make<Boolean>(false);
        }
    }
    return Heap::GetInstance().Make<Boolean>(true);
}

Object* IsEqual::Apply(Object* obj) const {
    return CompareInts::Apply(obj, [](int64_t a, int64_t b) { return a == b; });
}

Object* IsIncreasing::Apply(Object* obj) const {
    return CompareInts::Apply(obj, [](int64_t a, int64_t b) { return a <= b; });
}

Object* IsStrIncreasing::Apply(Object* obj) const {
    return CompareInts::Apply(obj, [](int64_t a, int64_t b) { return a < b; });
}

Object* IsDecreasing::Apply(Object* obj) const {
    return CompareInts::Apply(obj, [](int64_t a, int64_t b) { return a >= b; });
}

Object* IsStrDecreasing::Apply(Object* obj) const {
    return CompareInts::Apply(obj, [](int64_t a, int64_t b) { return a > b; });
}

Object* BinaryFunction::Apply(Object* obj, std::function<int64_t(int64_t, int64_t)>&& func) const {
    Params params;
    GetEvaluatedParams(obj, params);
    if (params.empty()) {
        throw RuntimeError("Got zero params");
    }
    if (!Is<Number>(params[0])) {
        throw RuntimeError("Got not a number param");
    }
    uint64_t x = As<Number>(params[0])->GetValue();
    for (size_t i = 1; i < params.size(); ++i) {
        if (const auto p = As<Number>(params[i])) {
            x = func(x, p->GetValue());
        } else {
            throw RuntimeError("Got not a number param");
        }
    }
    return Heap::GetInstance().Make<Number>(x);
}

Object* BinaryFunction::Apply(Object* obj, std::function<int64_t(int64_t, int64_t)>&& func,
                              uint64_t start) const {
    Params params;
    GetEvaluatedParams(obj, params);
    uint64_t x = start;
    for (size_t i = 0; i < params.size(); ++i) {
        if (const auto p = As<Number>(params[i])) {
            x = func(x, p->GetValue());
        } else {
            throw RuntimeError("Got not a number param");
        }
    }
    return Heap::GetInstance().Make<Number>(x);
}

Object* Sum::Apply(Object* obj) const {
    return BinaryFunction::Apply(
        obj, [](uint64_t a, uint64_t b) { return a + b; }, 0);
}

Object* Subtract::Apply(Object* obj) const {
    return BinaryFunction::Apply(obj, [](uint64_t a, uint64_t b) { return a - b; });
}

Object* Multiply::Apply(Object* obj) const {
    return BinaryFunction::Apply(
        obj, [](uint64_t a, uint64_t b) { return a * b; }, 1);
}

Object* Divide::Apply(Object* obj) const {
    return BinaryFunction::Apply(obj, [](uint64_t a, uint64_t b) { return a / b; });
}

Object* Max::Apply(Object* obj) const {
    return BinaryFunction::Apply(obj, [](uint64_t a, uint64_t b) { return std::max(a, b); });
}

Object* Min::Apply(Object* obj) const {
    return BinaryFunction::Apply(obj, [](uint64_t a, uint64_t b) { return std::min(a, b); });
}

Object* Abs::Apply(Object* obj) const {
    Params params;
    GetEvaluatedParams(obj, params);
    if (params.size() != 1) {
        throw RuntimeError("Wrong number of params");
    }
    if (!Is<Number>(params[0])) {
        throw RuntimeError("Got not a number param");
    }
    return Heap::GetInstance().Make<Number>(std::abs(As<Number>(params[0])->GetValue()));
}
