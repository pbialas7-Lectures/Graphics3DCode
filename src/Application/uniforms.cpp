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
        SPDLOG_INFO("Uniform block {} not found in program", name);
        return;
    }
    SPDLOG_INFO("Index of uniform block {} = {}", name, index);
    GLuint binding;
    OGL_CALL(glGetActiveUniformBlockiv(program, index, GL_UNIFORM_BLOCK_BINDING,
                                       reinterpret_cast<GLint *>(&binding)));
    SPDLOG_INFO("Uniform block {} binding = {}", name, binding);
    GLint size;
    OGL_CALL(glGetActiveUniformBlockiv(program, index, GL_UNIFORM_BLOCK_DATA_SIZE,
                                       &size));
    SPDLOG_INFO("Uniform block {} size = {}", name, size);

    GLint num_uniforms;
    OGL_CALL(glGetActiveUniformBlockiv(program, index, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS,
                                       &num_uniforms));
    SPDLOG_INFO("Uniform block {} num uniforms = {}", name, num_uniforms);
    GLint *uniform_indices = new GLint[num_uniforms];
    OGL_CALL(glGetActiveUniformBlockiv(program, index, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES,
                                       uniform_indices));
    for (int i = 0; i < num_uniforms; i++) {
        GLsizei length;
        GLenum type;
        GLchar name[32];
        GLint size;
        GLint uniform_index = uniform_indices[i];
        OGL_CALL(glGetActiveUniform(program, uniform_index, 32, &length, &size, &type, name));
        SPDLOG_INFO("Uniform block {} uniform {} name = {}", name, i, name);
        GLint offset;
        OGL_CALL(glGetActiveUniformsiv(program, 1, reinterpret_cast<GLuint *>(&uniform_index),
                                       GL_UNIFORM_OFFSET, &offset));
        SPDLOG_INFO("Uniform block {} uniform {} offset = {}", name, i, offset);



    }
}

