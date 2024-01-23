//
// Created by pbialas on 20.12.23.
//
#include "OGL/utils.h"

#include "glad/gl.h"

#include "spdlog/spdlog.h"

GLint xe::ogl::utils::get_uniform_location(GLuint program, const char *name) {
    GLint location = glGetUniformLocation(program, name);
    if (location == -1) {
        SPDLOG_WARN("Cannot find {} uniform", name);
    }
    return location;
}

