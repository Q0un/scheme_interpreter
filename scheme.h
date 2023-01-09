#pragma once

#include "object.h"
#include "scope.h"

#include <string>
#include <memory>

class Interpreter {
public:
    Interpreter();

    std::string Run(const std::string& scheme_str) const;

private:
    std::string Serialize(Object* obj) const;
    std::string SerializeList(Cell* cell) const;

    std::shared_ptr<Scope> global_scope_;
};
