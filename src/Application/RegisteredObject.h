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
        std::cout << "Registering " << this << std::endl;
        RegisteredObject::add(this);
    }

private:
    static std::list<RegisteredObject *> registry_;
};