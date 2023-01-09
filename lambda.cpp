#include "lambda.h"

#include "gc.h"

LambdaProducer::LambdaProducer(Object* body, std::vector<Object*>& vars,
                               std::shared_ptr<Scope> up_scope)
    : body_(body), up_scope_(up_scope) {
    for (const auto& i : vars) {
        if (const auto p = As<Symbol>(i)) {
            vars_.push_back(p->GetName());
        } else {
            throw RuntimeError("Lambda got not a symbol name of param");
        }
    }
    AddDependency(body_);
}

std::shared_ptr<IFunction> LambdaProducer::Produce(std::shared_ptr<Scope> scope) const {
    std::shared_ptr<Scope> new_scope = std::make_shared<Scope>(up_scope_);
    Heap::GetInstance().AddScope(new_scope);
    for (const auto& i : vars_) {
        new_scope->DefineVar(i, nullptr);
    }
    return std::make_shared<Lambda>(new_scope, body_, scope);
}

Lambda::Lambda(std::shared_ptr<Scope> scope, Object* body, std::shared_ptr<Scope> prev_scope)
    : IFunction(scope), body_(body), prev_scope_(prev_scope) {
}

Object* Lambda::Apply(Object* obj) const {
    Params params;
    GetEvaluatedParams(obj, params, prev_scope_);
    if (params.size() != scope_->CountVars()) {
        throw RuntimeError("Wrong number of params");
    }
    for (size_t i = 0; i < params.size(); ++i) {
        scope_->SetVar(i, params[i]);
    }
    Params functions;
    IFunction::GetEvaluatedParams(body_, functions);
    return functions.back();
}

void Lambda::GetEvaluatedParams(Object* obj, IFunction::Params& vec,
                                std::shared_ptr<Scope> fixed_scope) const {
    GetParams(obj, vec);
    for (auto& i : vec) {
        i = ApplyFunctions(i, fixed_scope);
    }
}
