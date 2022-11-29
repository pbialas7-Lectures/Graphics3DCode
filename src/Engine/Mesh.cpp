//
// Created by Piotr Białas on 12/11/2021.
//

#include <iostream>

#include "Mesh.h"

void xe::Mesh::draw() const {
    glBindVertexArray(vao_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, i_buffer_);
    for (auto i = 0; i < submeshes_.size(); i++) {
        glDrawElements(GL_TRIANGLES, submeshes_[i].count(), GL_UNSIGNED_SHORT,
                       reinterpret_cast<void *>(sizeof(GLushort) * submeshes_[i].start));
    }
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0u);
    glBindVertexArray(0u);
}

void xe::Mesh::vertex_attrib_pointer(GLuint index, GLuint size, GLenum type, GLsizei stride, GLsizei offset) {
    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, v_buffer_);
    glEnableVertexAttribArray(index);
    glVertexAttribPointer(index, size, type, GL_FALSE, stride, reinterpret_cast<void *>(offset));
    glBindBuffer(GL_ARRAY_BUFFER, 0u);
    glBindVertexArray(0u);
}


xe::Mesh::Mesh() {
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &v_buffer_);
    glGenBuffers(1, &i_buffer_);
    glBindVertexArray(vao_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, i_buffer_);
    glBindVertexArray(0u);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0u);
}

xe::Mesh::~Mesh() {
    glDeleteVertexArrays(1, &vao_);
    glDeleteBuffers(1, &v_buffer_);
    glDeleteBuffers(1, &i_buffer_);
}


void xe::Mesh::allocate_index_buffer(size_t size, GLenum hint) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, i_buffer_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, nullptr, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0u);
}

void xe::Mesh::load_indices(size_t offset, size_t size, void *data) {

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, i_buffer_);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0u);
}


void xe::Mesh::allocate_vertex_buffer(size_t size, GLenum hint) {
    glBindBuffer(GL_ARRAY_BUFFER, v_buffer_);
    glBufferData(GL_ARRAY_BUFFER, size, nullptr, hint);
    glBindBuffer(GL_ARRAY_BUFFER, 0u);
}

void xe::Mesh::load_vertices(size_t offset, size_t size, void *data) {
    glBindBuffer(GL_ARRAY_BUFFER, v_buffer_);
    glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
    glBindBuffer(GL_ARRAY_BUFFER, 0u);
}

