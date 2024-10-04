//
// Created by pbialas on 14.11.23.
//
#include "uniforms.h"

#include "spdlog/spdlog.h"
#include "spdlog/fmt/fmt.h"

#include "glad/gl.h"

#include "Application/utils.h"

void uniform_info(GLuint program, const char *name) {

    auto index = glGetUniformBlockIndex(program, name);
    if (index == -1) {
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
    GLint *uniform_indices = new GLint[num_uniforms];
    OGL_CALL(glGetActiveUniformBlockiv(program, index, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES,
                                       uniform_indices));
    for (int i = 0; i < num_uniforms; i++) {
        GLsizei length;
        GLenum type;
        GLchar u_name[32];
        GLint size;
        GLint uniform_index = uniform_indices[i];
        OGL_CALL(glGetActiveUniform(program, uniform_index, 32, &length, &size, &type, u_name));
        fmt::print("Uniform block {} uniform {} name = {}\n", name, i, u_name);
        GLint offset;
        OGL_CALL(glGetActiveUniformsiv(program, 1, reinterpret_cast<GLuint *>(&uniform_index),
                                       GL_UNIFORM_OFFSET, &offset));
        fmt::print("Uniform block {} uniform {} offset = {}\n", u_name, i, offset);
    }
}

