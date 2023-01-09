#pragma once

#include <memory>
#include <vector>

#include "object.h"

class Scope;

class IFunction {
public:
    IFunction(std::shared_ptr<Scope> scope);
    virtual ~IFunction() = default;
    virtual Object* Apply(Object* obj) const = 0;

protected:
    using Params = std::vector<Object*>;
    void GetParams(Object* obj, Params& vec) const;
    void GetEvaluatedParams(Object* obj, Params& vec) const;

    std::shared_ptr<Scope> scope_;
};

class Quote : public IFunction {
    using IFunction::IFunction;

public:
    Object* Apply(Object* obj) const override;
};

class IFunctionProducer {
public:
    virtual ~IFunctionProducer() = default;
    virtual std::shared_ptr<IFunction> Produce(std::shared_ptr<Scope> scope) const = 0;
};

template <typename Function>
class FunctionProducer : public IFunctionProducer {
public:
    std::shared_ptr<IFunction> Produce(std::shared_ptr<Scope> scope) const override {
        return std::make_shared<Function>(scope);
    }
};

Object* ApplyFunctions(Object* obj, std::shared_ptr<Scope> scope);
bool IsTrue(Object* obj);
