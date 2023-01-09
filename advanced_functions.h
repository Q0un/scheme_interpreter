#pragma once

#include "common_functions.h"

class If : public IFunction {
    using IFunction::IFunction;

public:
    Object* Apply(Object* obj) const override;
};

class Define : public IFunction {
    using IFunction::IFunction;

public:
    Object* Apply(Object* obj) const override;
};

class Set : public IFunction {
    using IFunction::IFunction;

public:
    Object* Apply(Object* obj) const override;
};

class SetCar : public IFunction {
    using IFunction::IFunction;

public:
    Object* Apply(Object* obj) const override;
};

class SetCdr : public IFunction {
    using IFunction::IFunction;

public:
    Object* Apply(Object* obj) const override;
};

class CreateLambda : public IFunction {
    using IFunction::IFunction;

public:
    Object* Apply(Object* obj) const override;
};

class IsSymbol : public IFunction {
    using IFunction::IFunction;

public:
    Object* Apply(Object* obj) const override;
};
