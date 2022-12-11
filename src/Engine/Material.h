//
// Created by Piotr Białas on 02/11/2021.
//

#pragma once

#include <glad/gl.h>
#include <glm/glm.hpp>

#include "Application/utils.h"
#include "Application/RegisteredObject.h"

namespace xe {

    class Material: public  RegisteredObject{
    public:
        virtual void bind() = 0;
        virtual void unbind() {};
    };

}


