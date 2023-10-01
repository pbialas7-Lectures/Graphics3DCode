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
    };

    extern const NullMaterial *null_material;

}


