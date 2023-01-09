#pragma once

#include <variant>
#include <optional>
#include <istream>
#include <memory>
#include <string>

struct SymbolToken {
    std::string name;

    bool operator==(const SymbolToken& other) const;
};

struct QuoteToken {
    bool operator==(const QuoteToken&) const;
};

struct DotToken {
    bool operator==(const DotToken&) const;
};

enum class BracketToken { OPEN, CLOSE };

struct ConstantToken {
    int value;

    bool operator==(const ConstantToken& other) const;
};

using Token = std::variant<ConstantToken, BracketToken, SymbolToken, QuoteToken, DotToken>;

class Tokenizer {
public:
    Tokenizer(std::istream* in);

    bool IsEnd();

    void Next();

    Token GetToken();

private:
    bool GoodStartSymbol(char c);
    bool GoodSymbol(char c);

    inline static const std::string kStartSymbols = "<=>*/#";
    inline static const std::string kSymbols = "<=>*/#?!-";
    std::istream* stream_;
    std::string seq_;
    bool is_ended_ = false;
};