//
// Created by pbialas on 20.12.23.
//

#pragma once

#include "glad/gl.h"

namespace xe {
    namespace ogl {

        namespace utils {
            GLint get_uniform_location(GLuint program, const char *name);
        }

    }
}