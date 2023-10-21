//
// Created by pbialas on 10.12.22.
//

#pragma once

#include "glad/gl.h"
#include "glm/glm.hpp"

namespace xe {
    void check_and_use_program(GLuint program);

    glm::vec4 get_color(const float c[3]);

    glm::vec3 srgb_inverse_gamma_correction(glm::vec3 color);
}