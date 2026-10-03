//
// Created by Piotr Białas on 02/11/2021.
//

#pragma once

#include <glad/gl.h>
#include <glm/glm.hpp>

#include "Application/utils.h"
#include "Application/RegisteredObject.h"

namespace xe {

    class Material : public RegisteredObject {
    public:
        virtual void bind() const = 0;

        virtual void unbind() const {};

    };


    class NullMaterial : public Material {
    public:

        void bind() const override {};

        // The singleton is deleted by RegisteredObject::cleanup(); forget it so a later call creates a new one.
        ~NullMaterial() override {
            if (null_material_ == this)
                null_material_ = nullptr;
        }

        static NullMaterial *null_material() {
            if(!null_material_)
                null_material_ = new NullMaterial;
            return null_material_;
        };

    private:
        static NullMaterial* null_material_;
    };


}


