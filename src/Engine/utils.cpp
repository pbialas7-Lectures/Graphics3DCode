//
// Created by pbialas on 10.12.22.
//
#include "Engine/utils.h"

#include "glm/glm.hpp"


namespace xe {
    void check_and_use_program(GLuint program) {
        GLint current_program_;
        glGetIntegerv(GL_CURRENT_PROGRAM, &current_program_);
        if (program != current_program_)
            glUseProgram(program);
    }


    glm::vec4 get_color(const float *c) {
        glm::vec4 color;
        for (int i = 0; i < 3; i++)
            color[i] = c[i];
        color[3] = 1.0;
        return color;
    }

    glm::vec3 srgb_inverse_gamma_correction(glm::vec3 color) {
        color = glm::clamp(color, 0.0f, 1.0f);
        color = glm::mix(color * 1.0f / 12.92f,
                         pow((color + 0.055f) / 1.055f, glm::vec3(2.4)), step(0.04045f, color));
        return color;
    }


}