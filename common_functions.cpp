#include "common_functions.h"

#include "error.h"
#include "scope.h"
#include "lambda.h"

Object* ApplyFunctions(Object* obj, std::shared_ptr<Scope> scope) {
    if (!obj) {
        throw RuntimeError("Got empty list");
    }
    if (const auto p = As<Symbol>(obj)) {
        return scope->GetVar(p);
    }
    if (!Is<Cell>(obj)) {
        return obj;
    }
    const auto cell = As<Cell>(obj);
    std::shared_ptr<IFunction> function;
    if (!Is<Symbol>(cell->GetFirst())) {
        try {
            const auto prod = As<LambdaProducer>(ApplyFunctions(cell->GetFirst(), scope));
            if (!prod) {
                throw RuntimeError("First element of list is not a function name");
            }
            function = prod->Produce(scope);
        } catch (...) {
            throw RuntimeError("First element of list is not a function name");
        }
    } else {
        try {
            function = scope->GetFunction(As<Symbol>(cell->GetFirst()));
        } catch (...) {
            throw RuntimeError("First element of list is not a function name");
        }
    }
    return function->Apply(cell->GetSecond());
}

IFunction::IFunction(std::shared_ptr<Scope> scope) : scope_(scope) {
}

void IFunction::GetParams(Object* obj, Params& vec) const {
    if (!obj) {
        return;
    }
    const auto cell = As<Cell>(obj);
    if (!cell) {
        vec.push_back(obj);
        return;
    }
    vec.push_back(cell->GetFirst());
    GetParams(cell->GetSecond(), vec);
}

void IFunction::GetEvaluatedParams(Object* obj, Params& vec) const {
    GetParams(obj, vec);
    for (auto& i : vec) {
        i = ApplyFunctions(i, scope_);
    }
}

Object* Quote::Apply(Object* obj) const {
    Params params;
    GetParams(obj, params);
    if (params.size() != 1) {
        throw RuntimeError("Wrong number of params");
    }
    return params[0];
}

bool IsTrue(Object* obj) {
    if (const auto p = As<Boolean>(obj)) {
        return p->GetValue();
    }
    return true;
}
