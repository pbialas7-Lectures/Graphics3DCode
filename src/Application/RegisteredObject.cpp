//
// Created by pbialas on 29.11.22.
//

#include "RegisteredObject.h"

std::list<RegisteredObject *> RegisteredObject::registry_;

void RegisteredObject::add(RegisteredObject *p) {
    registry_.push_back(p);
}

void RegisteredObject::cleanup() {
    for (auto p: registry_) {
        delete p;
    }
}
