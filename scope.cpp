#include "scope.h"

#include "lambda.h"

Scope::Scope(std::shared_ptr<Scope> up) : up_(up) {
}

auto Scope::FindVar(const std::string& name) {
    return std::find_if(vars_.begin(), vars_.end(), [&name](auto x) { return x.first == name; });
}

auto Scope::FindVar(const std::string& name) const {
    return std::find_if(vars_.begin(), vars_.end(), [&name](auto x) { return x.first == name; });
}

void Scope::DefineVar(const std::string& name, Object* value) {
    if (func_producers_.contains(name)) {
        func_producers_.erase(name);
    }
    auto it = FindVar(name);
    if (it != vars_.end()) {
        it->second = value;
    } else {
        vars_.emplace_back(name, value);
    }
}

void Scope::SetVar(const std::string& name, Object* value) {
    auto it = FindVar(name);
    if (it == vars_.end()) {
        if (up_) {
            up_->SetVar(name, value);
            return;
        }
        throw NameError("No such a variable");
    }
    it->second = value;
}

void Scope::SetVar(size_t index, Object* value) {
    if (index >= vars_.size()) {
        throw RuntimeError("There is no variable with such index");
    }
    vars_[index].second = value;
}

void Scope::DefineFuncProducer(const std::string& name, IFunctionProducer* func) {
    auto it = FindVar(name);
    if (it != vars_.end()) {
        vars_.erase(it);
    }
    func_producers_[name] = func;
}

void Scope::DefineVar(Symbol* name, Object* value) {
    DefineVar(name->GetName(), value);
}

void Scope::SetVar(Symbol* name, Object* value) {
    SetVar(name->GetName(), value);
}

void Scope::DefineFuncProducer(Symbol* name, IFunctionProducer* func) {
    DefineFuncProducer(name->GetName(), func);
}

Object* Scope::GetVar(const std::string& name) const {
    auto it = FindVar(name);
    if (it == vars_.end() || it->second == nullptr) {
        if (func_producers_.contains(name)) {
            if (const auto p = dynamic_cast<LambdaProducer*>(func_producers_.at(name))) {
                return p;
            }
        }
        if (up_) {
            return up_->GetVar(name);
        }
        throw NameError("No such a variable");
    }
    return it->second;
}

Object* Scope::GetVar(Symbol* name) const {
    return GetVar(name->GetName());
}

std::shared_ptr<IFunction> Scope::GetFunction(const std::string& name) {
    return GetFuncProducer(name)->Produce(shared_from_this());
}

std::shared_ptr<IFunction> Scope::GetFunction(Symbol* name) {
    return GetFunction(name->GetName());
}

size_t Scope::CountVars() const {
    return vars_.size();
}

IFunctionProducer* Scope::GetFuncProducer(const std::string& name) const {
    if (!func_producers_.contains(name) && up_) {
        return up_->GetFuncProducer(name);
    }
    try {
        return func_producers_.at(name);
    } catch (...) {
        throw NameError("No such a function");
    }
}

void Scope::Mark() {
    for (const auto& i : vars_) {
        i.second->Mark();
    }
    for (const auto& i : func_producers_) {
        if (const auto p = dynamic_cast<LambdaProducer*>(i.second)) {
            p->Mark();
        }
    }
}
