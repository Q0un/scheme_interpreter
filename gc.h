#pragma once

#include <vector>
#include <memory>

#include "object.h"
#include "error.h"
#include "scope.h"
#include "lambda.h"

#include <iostream>

class Heap {
public:
    Heap() = default;
    ~Heap();

    static Heap& GetInstance();

    static void ResetInstance();

    template <typename T, typename... Args,
              std::enable_if_t<std::is_base_of_v<Object, T>>* = nullptr>
    T* Make(Args&&... args) {
        auto ptr = new T(std::forward<Args>(args)...);
        objects_.push_back(ptr);
        return ptr;
    }

    template <typename T, typename... Args,
              std::enable_if_t<std::is_base_of_v<IFunctionProducer, T>>* = nullptr,
              std::enable_if_t<!std::is_same_v<T, LambdaProducer>>* = nullptr>
    T* Make(Args&&... args) {
        auto ptr = new T(std::forward<Args>(args)...);
        prods_.push_back(ptr);
        return ptr;
    }

    void AddScope(std::weak_ptr<Scope> scope);

    void Clear();

private:
    std::vector<Object*> objects_;
    std::vector<IFunctionProducer*> prods_;
    std::vector<std::weak_ptr<Scope>> scopes_;

    static std::unique_ptr<Heap> heap;
};
