//
// Created by Piotr Białas on 12/11/2021.
//

#include <iostream>

#include "Mesh.h"

#include "spdlog/spdlog.h"

#include "Application/utils.h"

xe::Mesh::Mesh(GLsizei stride, GLsizei v_buffer_size, GLenum v_buffer_hint,
               GLsizei i_buffer_size, GLenum index_type, GLenum i_buffer_hint) :
        stride_(stride), index_type_(index_type) {
    glGenVertexArrays(1, &vao_);

    glGenBuffers(1, &v_buffer_);
    glGenBuffers(1, &i_buffer_);
    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, v_buffer_);
    glBufferData(GL_ARRAY_BUFFER, v_buffer_size, nullptr, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0u);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, i_buffer_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, i_buffer_size, nullptr, GL_STATIC_DRAW);
    glBindVertexArray(0u);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0u);

    switch (index_type_) {
        case GL_UNSIGNED_BYTE:
            index_size_ = 1;
            break;
        case GL_UNSIGNED_SHORT:
            index_size_ = 2;
            break;
        case GL_UNSIGNED_INT:
            index_size_ = 4;
            break;
    }
}


void xe::Mesh::load_indices(size_t offset, size_t size, void *data) {

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, i_buffer_);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0u);
}

void xe::Mesh::load_vertices(size_t offset, size_t size, void *data) {
    glBindBuffer(GL_ARRAY_BUFFER, v_buffer_);
    glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
    glBindBuffer(GL_ARRAY_BUFFER, 0u);
}


void xe::Mesh::add_attribute(xe::AttributeType attr_type, GLuint size, GLenum type, GLsizei offset) const {
    auto index = static_cast<int>(attr_type);
    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, v_buffer_);
    glEnableVertexAttribArray(index);
    glVertexAttribPointer(index, size, type, GL_FALSE, stride_, reinterpret_cast<void *>(offset));
    glBindBuffer(GL_ARRAY_BUFFER, 0u);
    glBindVertexArray(0u);
}

void xe::Mesh::draw() const {
    glBindVertexArray(vao_);
    for (auto i = 0; i < primitives_.size(); i++) {
        primitives_[i].material->bind();
        auto indices = reinterpret_cast<void *>(index_size_ * primitives_[i].start);
        OGL_CALL(glDrawElements(GL_TRIANGLES, primitives_[i].count(), index_type_, indices));
        primitives_[i].material->unbind();
    }
    glBindVertexArray(0u);
}






