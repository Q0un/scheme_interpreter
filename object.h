#pragma once

#include <memory>
#include <string>
#include <unordered_set>

class Object {
    friend class Heap;
    friend class Scope;

public:
    virtual ~Object() = default;

protected:
    void AddDependency(Object* obj) {
        if (!obj) {
            return;
        }
        depends_.insert(obj);
    }

    void RemoveDependency(Object* obj) {
        if (!obj) {
            return;
        }
        depends_.erase(obj);
    }

private:
    std::unordered_set<Object*> depends_;
    bool marked_ = false;

    void Mark() {
        marked_ = true;
        for (const auto& i : depends_) {
            if (!i->marked_) {
                i->Mark();
            }
        }
    }
};

class Number : public Object {
public:
    Number(int64_t value) : value_(value) {
    }

    int64_t GetValue() const {
        return value_;
    }

private:
    int64_t value_;
};

class Symbol : public Object {
public:
    Symbol(std::string_view name) : name_(name) {
    }

    const std::string& GetName() const {
        return name_;
    }

private:
    std::string name_;
};

class Cell : public Object {
public:
    Cell() = default;

    Cell(Object* first, Object* second) : first_(first), second_(second) {
        AddDependency(first_);
        AddDependency(second_);
    }

    Object* GetFirst() const {
        return first_;
    }
    Object* GetSecond() const {
        return second_;
    }

    void SetFirst(Object* obj) {
        RemoveDependency(first_);
        first_ = obj;
        AddDependency(first_);
    }

    void SetSecond(Object* obj) {
        RemoveDependency(second_);
        second_ = obj;
        AddDependency(second_);
    }

private:
    Object* first_;
    Object* second_;
};

class Boolean : public Object {
public:
    Boolean() = default;

    Boolean(bool value) : value_(value) {
    }

    bool GetValue() const {
        return value_;
    }

private:
    bool value_;
};

///////////////////////////////////////////////////////////////////////////////

// Runtime type checking and convertion.
// This can be helpful: https://en.cppreference.com/w/cpp/memory/shared_ptr/pointer_cast

template <class T>
T* As(Object* obj) {
    return dynamic_cast<T*>(obj);
}

template <class T>
bool Is(Object* obj) {
    return dynamic_cast<T*>(obj) != nullptr;
}
