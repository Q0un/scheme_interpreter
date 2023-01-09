#pragma once

#include <vector>

#include "object.h"
#include "scope.h"
#include "common_functions.h"

class LambdaProducer : public IFunctionProducer, public Object {
public:
    LambdaProducer(Object* body, std::vector<Object*>& vars, std::shared_ptr<Scope> up_scope);

    std::shared_ptr<IFunction> Produce(std::shared_ptr<Scope> scope) const override;

private:
    Object* body_;
    std::vector<std::string> vars_;
    std::shared_ptr<Scope> up_scope_;
};

class Lambda : public IFunction {
public:
    Lambda() = default;
    Lambda(std::shared_ptr<Scope> scope, Object* body, std::shared_ptr<Scope> prev_scope);

    Object* Apply(Object* obj) const override;
    void GetEvaluatedParams(Object* obj, Params& vec, std::shared_ptr<Scope> fixed_scope) const;

private:
    Object* body_;
    std::shared_ptr<Scope> prev_scope_;
};
