#pragma once

#include "common_functions.h"

class IsBoolean : public IFunction {
    using IFunction::IFunction;

public:
    Object* Apply(Object* obj) const override;
};

class Not : public IFunction {
    using IFunction::IFunction;

public:
    Object* Apply(Object* obj) const override;
};

class And : public IFunction {
    using IFunction::IFunction;

public:
    Object* Apply(Object* obj) const override;
};

class Or : public IFunction {
    using IFunction::IFunction;

public:
    Object* Apply(Object* obj) const override;
};
