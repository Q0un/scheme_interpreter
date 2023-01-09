#include "parser.h"
#include "error.h"
#include "gc.h"

Cell* ReadList(Tokenizer* tokenizer) {
    auto token = tokenizer->GetToken();
    if (const auto* p = std::get_if<BracketToken>(&token); p) {
        if (*p == BracketToken::CLOSE) {
            tokenizer->Next();
            return nullptr;
        }
    }

    Cell cell;
    auto first = Read(tokenizer);

    token = tokenizer->GetToken();
    if (const auto* p = std::get_if<DotToken>(&token); p) {
        tokenizer->Next();
        auto second = Read(tokenizer);

        token = tokenizer->GetToken();
        if (const auto* p = std::get_if<BracketToken>(&token); p) {
            if (*p != BracketToken::CLOSE) {
                throw SyntaxError("Closing bracket not found");
            }
            tokenizer->Next();
        } else {
            throw SyntaxError("Closing bracket not found");
        }
        return Heap::GetInstance().Make<Cell>(first, second);
    }

    auto second = ReadList(tokenizer);
    return Heap::GetInstance().Make<Cell>(first, second);
}

Object* Read(Tokenizer* tokenizer) {
    if (tokenizer->IsEnd()) {
        throw SyntaxError("There is no token to read");
    }
    const auto token = tokenizer->GetToken();
    tokenizer->Next();
    if (const auto* p = std::get_if<SymbolToken>(&token); p) {
        if (p->name == "#t") {
            return Heap::GetInstance().Make<Boolean>(true);
        }
        if (p->name == "#f") {
            return Heap::GetInstance().Make<Boolean>(false);
        }
        return Heap::GetInstance().Make<Symbol>(p->name);
    }
    if (const auto* p = std::get_if<QuoteToken>(&token); p) {
        auto first = Heap::GetInstance().Make<Symbol>("quote");
        auto second = Read(tokenizer);
        return Heap::GetInstance().Make<Cell>(first,
                                              Heap::GetInstance().Make<Cell>(second, nullptr));
    }
    if (const auto* p = std::get_if<DotToken>(&token); p) {
        throw SyntaxError("Parser got unexpected dot token");
    }
    if (const auto* p = std::get_if<BracketToken>(&token); p) {
        if (*p == BracketToken::CLOSE) {
            throw SyntaxError("Parser got unexpected closing bracket token");
        }
        return ReadList(tokenizer);
    }
    if (const auto* p = std::get_if<ConstantToken>(&token); p) {
        return Heap::GetInstance().Make<Number>(p->value);
    }
    throw RuntimeError("Parser got invalid token");
}