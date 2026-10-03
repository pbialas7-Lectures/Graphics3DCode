//
// Created by pbialas on 29.11.22.
//

#include "RegisteredObject.h"

std::list<RegisteredObject *> RegisteredObject::registry_;

void RegisteredObject::add(RegisteredObject *p) {
    registry_.push_back(p);
}

void RegisteredObject::remove(RegisteredObject *p) {
    registry_.remove(p);
}

void RegisteredObject::cleanup() {
    // Move the registry out first, so destructors calling remove() do not modify the list we iterate over
    // and a second call to cleanup() does not delete the objects again.
    std::list<RegisteredObject *> objects;
    objects.swap(registry_);
    for (auto p: objects) {
        delete p;
    }
}
