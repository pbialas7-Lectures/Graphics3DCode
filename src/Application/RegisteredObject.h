//
// Created by pbialas on 29.11.22.
//
#pragma  once

#include <list>

// Objects derived from this class are deleted by RegisteredObject::cleanup(),
// so they must be allocated with new.
class RegisteredObject {


public:
    static void add(RegisteredObject *p);

    static void remove(RegisteredObject *p);

    static void cleanup();

    RegisteredObject() {
        RegisteredObject::add(this);
    }

    virtual ~RegisteredObject() {
        RegisteredObject::remove(this);
    }


private:
    static std::list<RegisteredObject *> registry_;
};
