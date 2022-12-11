//
// Created by pbialas on 10.12.22.
//
#include "Engine/utils.h"

void check_and_use_program(GLuint program) {
    GLint current_program_;
    glGetIntegerv(GL_CURRENT_PROGRAM, &current_program_);
    if(program!=current_program_)
        glUseProgram(program);
}
