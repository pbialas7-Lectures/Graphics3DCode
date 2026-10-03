//
// Created by pbialas on 14.11.23.
//
#include "uniforms.h"

#include <vector>

#include "spdlog/spdlog.h"
#include "spdlog/fmt/fmt.h"

#include "glad/gl.h"

#include "Application/utils.h"

void uniform_info(GLuint program, const char *name) {

    auto index = glGetUniformBlockIndex(program, name);
    if (index == GL_INVALID_INDEX) {
        fmt::print("Uniform block {} not found in program\n", name);
        return;
    }

    fmt::print("Index of uniform block {} = {}\n", name, index);
    GLuint binding;
    OGL_CALL(glGetActiveUniformBlockiv(program, index, GL_UNIFORM_BLOCK_BINDING,
                                       reinterpret_cast<GLint *>(&binding)));
    fmt::print("Uniform block {} binding = {}\n", name, binding);
    GLint size;
    OGL_CALL(glGetActiveUniformBlockiv(program, index, GL_UNIFORM_BLOCK_DATA_SIZE,
                                       &size));
    fmt::print("Uniform block {} size = {}\n", name, size);
    GLint num_uniforms;
    OGL_CALL(glGetActiveUniformBlockiv(program, index, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS,
                                       &num_uniforms));
    fmt::print("Uniform block {} num uniforms = {}\n", name, num_uniforms);
    std::vector<GLint> uniform_indices(num_uniforms);
    GLint max_name_length;
    OGL_CALL(glGetProgramiv(program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &max_name_length));
    std::vector<GLchar> u_name(max_name_length);
    OGL_CALL(glGetActiveUniformBlockiv(program, index, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES,
                                       uniform_indices.data()));
    for (int i = 0; i < num_uniforms; i++) {
        GLsizei length;
        GLenum type;
        GLint size;
        GLint uniform_index = uniform_indices[i];
        OGL_CALL(glGetActiveUniform(program, uniform_index, max_name_length, &length, &size, &type, u_name.data()));
        fmt::print("Uniform block {} uniform {} name = {}\n", name, i, u_name.data());
        GLint offset;
        OGL_CALL(glGetActiveUniformsiv(program, 1, reinterpret_cast<GLuint *>(&uniform_index),
                                       GL_UNIFORM_OFFSET, &offset));
        fmt::print("Uniform block {} uniform {} offset = {}\n", name, i, offset);
    }
}

