//
// Created by pbialas on 29.11.22.
//
#pragma  once

#include <iostream>
#include <list>

class RegisteredObject {


public:
    static void add(RegisteredObject *p);

    static void cleanup();

    RegisteredObject() {
        RegisteredObject::add(this);
    }

    virtual ~RegisteredObject() {}


private:
    static std::list<RegisteredObject *> registry_;
};