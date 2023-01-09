#include "advanced_functions.h"

#include "error.h"
#include "lambda.h"
#include "gc.h"

Object* If::Apply(Object* obj) const {
    Params params;
    GetParams(obj, params);
    if (params.size() < 2 || params.size() > 3) {
        throw SyntaxError("Wrong number of params");
    }
    params[0] = ApplyFunctions(params[0], scope_);
    if (IsTrue(params[0])) {
        return ApplyFunctions(params[1], scope_);
    }
    if (params.size() == 2) {
        return nullptr;
    }
    return ApplyFunctions(params[2], scope_);
}

Object* Define::Apply(Object* obj) const {
    Params params;
    GetParams(obj, params);
    const auto cell = As<Cell>(obj);
    if (params.size() < 2) {
        throw SyntaxError("Wrong number of params");
    }
    if (const auto name = As<Symbol>(params[0])) {
        if (params.size() != 2) {
            throw SyntaxError("Wrong number of params");
        }
        const auto value = ApplyFunctions(params[1], scope_);
        if (const auto prod = As<LambdaProducer>(value)) {
            scope_->DefineFuncProducer(name, prod);
        } else {
            scope_->DefineVar(name, value);
        }
        return nullptr;
    }
    if (const auto decl = As<Cell>(params[0])) {
        const auto name = As<Symbol>(decl->GetFirst());
        if (name == nullptr) {
            throw RuntimeError("Wrong types of params");
        }
        Heap& heap = Heap::GetInstance();
        const auto lambda = heap.Make<Cell>(heap.Make<Symbol>("lambda"),
                                            heap.Make<Cell>(decl->GetSecond(), cell->GetSecond()));
        const auto prod = As<LambdaProducer>(ApplyFunctions(lambda, scope_));
        scope_->DefineFuncProducer(name, prod);
        return nullptr;
    }
    throw RuntimeError("Wrong types of params");
}

Object* Set::Apply(Object* obj) const {
    Params params;
    GetParams(obj, params);
    if (params.size() != 2) {
        throw SyntaxError("Wrong number of params");
    }
    if (const auto p = As<Symbol>(params[0])) {
        scope_->SetVar(p->GetName(), ApplyFunctions(params[1], scope_));
        return nullptr;
    }
    throw RuntimeError("Wrong types of params");
}

Object* SetCar::Apply(Object* obj) const {
    Params params;
    GetEvaluatedParams(obj, params);
    if (params.size() != 2) {
        throw SyntaxError("Wrong number of params");
    }
    if (!Is<Cell>(params[0])) {
        throw RuntimeError("Wrong types of params");
    }
    auto cell = As<Cell>(params[0]);
    cell->SetFirst(params[1]);
    return nullptr;
}

Object* SetCdr::Apply(Object* obj) const {
    Params params;
    GetEvaluatedParams(obj, params);
    if (params.size() != 2) {
        throw SyntaxError("Wrong number of params");
    }
    if (!Is<Cell>(params[0])) {
        throw RuntimeError("Wrong types of params");
    }
    auto cell = As<Cell>(params[0]);
    cell->SetSecond(params[1]);
    return nullptr;
}

Object* CreateLambda::Apply(Object* obj) const {
    Params params;
    GetParams(obj, params);
    if (params.size() < 2) {
        throw SyntaxError("Wrong number of params");
    }
    Params vars;
    GetParams(params[0], vars);
    const auto p = As<Cell>(obj);
    return Heap::GetInstance().Make<LambdaProducer>(p->GetSecond(), vars, scope_);
}

Object* IsSymbol::Apply(Object* obj) const {
    Params params;
    GetEvaluatedParams(obj, params);
    if (params.size() != 1) {
        throw SyntaxError("Wrong number of params");
    }
    return Heap::GetInstance().Make<Boolean>(Is<Symbol>(params[0]));
}
