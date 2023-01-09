#pragma once

#include <string>
#include <memory>
#include <unordered_map>
#include <vector>

#include "object.h"
#include "error.h"
#include "common_functions.h"

class Scope : public std::enable_shared_from_this<Scope> {
    friend class Heap;

public:
    Scope() = default;
    Scope(std::shared_ptr<Scope> up);

    void DefineVar(const std::string& name, Object* value);
    void SetVar(const std::string& name, Object* value);
    void SetVar(size_t index, Object* value);
    void DefineFuncProducer(const std::string& name, IFunctionProducer* func);
    void DefineVar(Symbol* name, Object* value);
    void SetVar(Symbol* name, Object* value);
    void DefineFuncProducer(Symbol* name, IFunctionProducer* func);

    Object* GetVar(const std::string& name) const;
    Object* GetVar(Symbol* name) const;
    std::shared_ptr<IFunction> GetFunction(const std::string& name);
    std::shared_ptr<IFunction> GetFunction(Symbol* name);

    size_t CountVars() const;

private:
    IFunctionProducer* GetFuncProducer(const std::string& name) const;
    auto FindVar(const std::string& name);
    auto FindVar(const std::string& name) const;
    void Mark();

    std::vector<std::pair<std::string, Object*>> vars_;
    std::unordered_map<std::string, IFunctionProducer*> func_producers_;
    std::shared_ptr<Scope> up_;
};
