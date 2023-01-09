#include "gc.h"

#include "lambda.h"

std::unique_ptr<Heap> Heap::heap;

Heap::~Heap() {
    for (const auto& i : objects_) {
        delete i;
    }
    for (const auto& i : prods_) {
        delete i;
    }
}

Heap& Heap::GetInstance() {
    if (!heap) {
        heap = std::make_unique<Heap>();
    }
    return *heap;
}

void Heap::ResetInstance() {
    heap = std::make_unique<Heap>();
}

void Heap::Clear() {
    for (const auto& i : objects_) {
        i->marked_ = false;
    }
    size_t i = 0;
    while (i < scopes_.size()) {
        if (!scopes_[i].expired()) {
            scopes_[i].lock()->Mark();
            ++i;
        } else {
            std::swap(scopes_[i], scopes_.back());
            scopes_.pop_back();
        }
    }
    i = 0;
    while (i < objects_.size()) {
        if (!objects_[i]->marked_) {
            delete objects_[i];
            std::swap(objects_[i], objects_.back());
            objects_.pop_back();
        } else {
            ++i;
        }
    }
}

void Heap::AddScope(std::weak_ptr<Scope> scope) {
    scopes_.push_back(scope);
}
