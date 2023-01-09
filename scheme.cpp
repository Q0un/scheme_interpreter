#include "scheme.h"
#include "tokenizer.h"
#include "parser.h"
#include "error.h"
#include "gc.h"

#include "advanced_functions.h"
#include "boolean_functions.h"
#include "common_functions.h"
#include "integer_functions.h"
#include "list_functions.h"

#include <sstream>

Interpreter::Interpreter() {
    Heap::ResetInstance();

    global_scope_ = std::make_shared<Scope>();
    Heap& heap = Heap::GetInstance();
    heap.AddScope(global_scope_);
    global_scope_->DefineFuncProducer("quote", heap.Make<FunctionProducer<Quote>>());
    global_scope_->DefineFuncProducer("number?", heap.Make<FunctionProducer<IsNumber>>());
    global_scope_->DefineFuncProducer("=", heap.Make<FunctionProducer<IsEqual>>());
    global_scope_->DefineFuncProducer("<=", heap.Make<FunctionProducer<IsIncreasing>>());
    global_scope_->DefineFuncProducer("<", heap.Make<FunctionProducer<IsStrIncreasing>>());
    global_scope_->DefineFuncProducer(">=", heap.Make<FunctionProducer<IsDecreasing>>());
    global_scope_->DefineFuncProducer(">", heap.Make<FunctionProducer<IsStrDecreasing>>());
    global_scope_->DefineFuncProducer("+", heap.Make<FunctionProducer<Sum>>());
    global_scope_->DefineFuncProducer("-", heap.Make<FunctionProducer<Subtract>>());
    global_scope_->DefineFuncProducer("*", heap.Make<FunctionProducer<Multiply>>());
    global_scope_->DefineFuncProducer("/", heap.Make<FunctionProducer<Divide>>());
    global_scope_->DefineFuncProducer("max", heap.Make<FunctionProducer<Max>>());
    global_scope_->DefineFuncProducer("min", heap.Make<FunctionProducer<Min>>());
    global_scope_->DefineFuncProducer("abs", heap.Make<FunctionProducer<Abs>>());
    global_scope_->DefineFuncProducer("boolean?", heap.Make<FunctionProducer<IsBoolean>>());
    global_scope_->DefineFuncProducer("not", heap.Make<FunctionProducer<Not>>());
    global_scope_->DefineFuncProducer("and", heap.Make<FunctionProducer<And>>());
    global_scope_->DefineFuncProducer("or", heap.Make<FunctionProducer<Or>>());
    global_scope_->DefineFuncProducer("pair?", heap.Make<FunctionProducer<IsPair>>());
    global_scope_->DefineFuncProducer("null?", heap.Make<FunctionProducer<IsNull>>());
    global_scope_->DefineFuncProducer("list?", heap.Make<FunctionProducer<IsList>>());
    global_scope_->DefineFuncProducer("cons", heap.Make<FunctionProducer<Cons>>());
    global_scope_->DefineFuncProducer("car", heap.Make<FunctionProducer<Car>>());
    global_scope_->DefineFuncProducer("cdr", heap.Make<FunctionProducer<Cdr>>());
    global_scope_->DefineFuncProducer("list", heap.Make<FunctionProducer<List>>());
    global_scope_->DefineFuncProducer("list-ref", heap.Make<FunctionProducer<ListRef>>());
    global_scope_->DefineFuncProducer("list-tail", heap.Make<FunctionProducer<ListTail>>());
    global_scope_->DefineFuncProducer("if", heap.Make<FunctionProducer<If>>());
    global_scope_->DefineFuncProducer("define", heap.Make<FunctionProducer<Define>>());
    global_scope_->DefineFuncProducer("set!", heap.Make<FunctionProducer<Set>>());
    global_scope_->DefineFuncProducer("set-car!", heap.Make<FunctionProducer<SetCar>>());
    global_scope_->DefineFuncProducer("set-cdr!", heap.Make<FunctionProducer<SetCdr>>());
    global_scope_->DefineFuncProducer("lambda", heap.Make<FunctionProducer<CreateLambda>>());
    global_scope_->DefineFuncProducer("symbol?", heap.Make<FunctionProducer<IsSymbol>>());
}

std::string Interpreter::Run(const std::string& scheme_str) const {
    std::stringstream ss{scheme_str};
    Tokenizer tokenizer(&ss);
    auto scheme = Read(&tokenizer);
    if (!tokenizer.IsEnd()) {
        throw SyntaxError("Wrong input string");
    }
    auto res = Serialize(ApplyFunctions(scheme, global_scope_));
    Heap::GetInstance().Clear();
    return res;
}

std::string Interpreter::Serialize(Object* obj) const {
    if (!obj) {
        return "()";
    }
    if (const auto p = As<Symbol>(obj)) {
        return p->GetName();
    }
    if (const auto p = As<Number>(obj)) {
        return std::to_string(p->GetValue());
    }
    if (const auto p = As<Cell>(obj)) {
        return "(" + SerializeList(p);
    }
    if (const auto p = As<Boolean>(obj)) {
        return p->GetValue() ? "#t" : "#f";
    }
    throw RuntimeError("Got unexpected object");
}

std::string Interpreter::SerializeList(Cell* cell) const {
    if (!cell->GetSecond()) {
        return Serialize(cell->GetFirst()) + ")";
    }
    if (const auto p = As<Cell>(cell->GetSecond())) {
        return Serialize(cell->GetFirst()) + " " + SerializeList(p);
    }
    return Serialize(cell->GetFirst()) + " . " + Serialize(cell->GetSecond()) + ")";
}
