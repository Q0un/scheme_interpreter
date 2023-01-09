#include "tokenizer.h"
#include "error.h"

bool SymbolToken::operator==(const SymbolToken& other) const {
    return name == other.name;
}

bool QuoteToken::operator==(const QuoteToken&) const {
    return true;
}

bool DotToken::operator==(const DotToken&) const {
    return true;
}

bool ConstantToken::operator==(const ConstantToken& other) const {
    return value == other.value;
}

Tokenizer::Tokenizer(std::istream* in) : stream_(in) {
    Next();
}

bool Tokenizer::IsEnd() {
    return is_ended_;
}

void Tokenizer::Next() {
    seq_.clear();
    char c = stream_->get();
    while (!stream_->eof() && (c == ' ' || c == '\n')) {
        c = stream_->get();
    }
    if (stream_->eof()) {
        is_ended_ = true;
        return;
    }
    if (c == '+' || c == '-' || std::isdigit(c)) {
        seq_ += c;
        c = stream_->peek();
        while (!stream_->eof() && std::isdigit(c)) {
            seq_ += stream_->get();
            c = stream_->peek();
        }
        return;
    }
    if (c == '(' || c == ')' || c == '\'' || c == '.') {
        seq_ += c;
        return;
    }
    if (GoodStartSymbol(c)) {
        seq_ += c;
        c = stream_->peek();
        while (!stream_->eof() && GoodSymbol(c)) {
            seq_ += stream_->get();
            c = stream_->peek();
        }
        return;
    }
    throw SyntaxError("Wrong symbol while reading");
}

Token Tokenizer::GetToken() {
    if (seq_[0] == '(') {
        return BracketToken::OPEN;
    }
    if (seq_[0] == ')') {
        return BracketToken::CLOSE;
    }
    if (seq_[0] == '\'') {
        return QuoteToken{};
    }
    if (seq_[0] == '.') {
        return DotToken{};
    }
    try {
        return ConstantToken{std::stoi(seq_)};
    } catch (const std::invalid_argument& ex) {
        return SymbolToken{seq_};
    }
}

bool Tokenizer::GoodStartSymbol(char c) {
    return std::isalpha(c) || kStartSymbols.find(c) != kStartSymbols.npos;
}

bool Tokenizer::GoodSymbol(char c) {
    return std::isalnum(c) || kSymbols.find(c) != kSymbols.npos;
}
