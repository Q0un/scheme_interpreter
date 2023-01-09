#pragma once

#include "common_functions.h"

class IsPair : public IFunction {
    using IFunction::IFunction;

public:
    Object* Apply(Object* obj) const override;
};

class IsNull : public IFunction {
    using IFunction::IFunction;

public:
    Object* Apply(Object* obj) const override;
};

class IsList : public IFunction {
    using IFunction::IFunction;

public:
    Object* Apply(Object* obj) const override;
};

class Cons : public IFunction {
    using IFunction::IFunction;

public:
    Object* Apply(Object* obj) const override;
};

class Car : public IFunction {
    using IFunction::IFunction;

public:
    Object* Apply(Object* obj) const override;
};

class Cdr : public IFunction {
    using IFunction::IFunction;

public:
    Object* Apply(Object* obj) const override;
};

class List : public IFunction {
    using IFunction::IFunction;

public:
    Object* Apply(Object* obj) const override;
};

class ListRef : public IFunction {
    using IFunction::IFunction;

public:
    Object* Apply(Object* obj) const override;
};

class ListTail : public IFunction {
    using IFunction::IFunction;

public:
    Object* Apply(Object* obj) const override;
};
