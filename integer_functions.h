#pragma once

#include <memory>
#include <functional>

#include "object.h"
#include "common_functions.h"

class IsNumber : public IFunction {
    using IFunction::IFunction;

public:
    Object* Apply(Object* obj) const override;
};

class CompareInts : public IFunction {
    using IFunction::IFunction;

public:
    Object* Apply(Object* obj, std::function<bool(int64_t, int64_t)>&& func) const;
};

class IsEqual : public CompareInts {
    using CompareInts::CompareInts;

public:
    Object* Apply(Object* obj) const override;
};

class IsIncreasing : public CompareInts {
    using CompareInts::CompareInts;

public:
    Object* Apply(Object* obj) const override;
};

class IsStrIncreasing : public CompareInts {
    using CompareInts::CompareInts;

public:
    Object* Apply(Object* obj) const override;
};

class IsDecreasing : public CompareInts {
    using CompareInts::CompareInts;

public:
    Object* Apply(Object* obj) const override;
};

class IsStrDecreasing : public CompareInts {
    using CompareInts::CompareInts;

public:
    Object* Apply(Object* obj) const override;
};

class BinaryFunction : public IFunction {
    using IFunction::IFunction;

public:
    Object* Apply(Object* obj, std::function<int64_t(int64_t, int64_t)>&& func) const;
    Object* Apply(Object* obj, std::function<int64_t(int64_t, int64_t)>&& func,
                  uint64_t start) const;
};

class Sum : public BinaryFunction {
    using BinaryFunction::BinaryFunction;

public:
    Object* Apply(Object* obj) const override;
};

class Subtract : public BinaryFunction {
    using BinaryFunction::BinaryFunction;

public:
    Object* Apply(Object* obj) const override;
};

class Multiply : public BinaryFunction {
    using BinaryFunction::BinaryFunction;

public:
    Object* Apply(Object* obj) const override;
};

class Divide : public BinaryFunction {
    using BinaryFunction::BinaryFunction;

public:
    Object* Apply(Object* obj) const override;
};

class Max : public BinaryFunction {
    using BinaryFunction::BinaryFunction;

public:
    Object* Apply(Object* obj) const override;
};

class Min : public BinaryFunction {
    using BinaryFunction::BinaryFunction;

public:
    Object* Apply(Object* obj) const override;
};

class Abs : public IFunction {
    using IFunction::IFunction;

public:
    Object* Apply(Object* obj) const override;
};
