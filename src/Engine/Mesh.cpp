//
// Created by Piotr Białas on 12/11/2021.
//

#include <iostream>

#include "Mesh.h"

#include "spdlog/spdlog.h"

#include "Application/utils.h"

namespace xe {
    Mesh::Mesh(GLsizei stride, GLsizei v_buffer_size, GLenum v_buffer_hint,
               GLsizei i_buffer_size, GLenum index_type, GLenum i_buffer_hint) :
            stride_(stride), index_type_(index_type) {
        OGL_CALL(glGenVertexArrays(1, vao_.put()));

        OGL_CALL(glCreateBuffers(1, v_buffer_.put()));
        OGL_CALL(glNamedBufferData(v_buffer_.get(), v_buffer_size, nullptr, v_buffer_hint));

        OGL_CALL(glCreateBuffers(1, i_buffer_.put()));
        OGL_CALL(glNamedBufferData(i_buffer_.get(), i_buffer_size, nullptr, i_buffer_hint));

        OGL_CALL(glBindVertexArray(vao_.get()));
        OGL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, i_buffer_.get()));
        OGL_CALL(glBindVertexArray(0u));
        OGL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0u));

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
            default:
                SPDLOG_ERROR("Unsupported index type {:#x}, use GL_UNSIGNED_BYTE, GL_UNSIGNED_SHORT or GL_UNSIGNED_INT",
                             index_type_);
                index_size_ = 0;
                break;
        }
    }


    void Mesh::load_indices(size_t offset, size_t size, void *data) {
        OGL_CALL(glNamedBufferSubData(i_buffer_.get(), offset, size, data));
    }

    void Mesh::load_vertices(size_t offset, size_t size, void *data) {
        OGL_CALL(glNamedBufferSubData(v_buffer_.get(), offset, size, data));
    }


    void Mesh::add_attribute(xe::AttributeType attr_type, GLuint size, GLenum type, GLsizei offset) const {
        auto index = static_cast<int>(attr_type);
        OGL_CALL(glBindVertexArray(vao_.get()));
        OGL_CALL(glBindBuffer(GL_ARRAY_BUFFER, v_buffer_.get()));
        OGL_CALL(glEnableVertexAttribArray(index));
        OGL_CALL(glVertexAttribPointer(index, size, type, GL_FALSE, stride_, reinterpret_cast<void *>(offset)));
        OGL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0u));
        OGL_CALL(glBindVertexArray(0u));
    }

    void Mesh::draw() const {
        OGL_CALL(glBindVertexArray(vao_.get()));
        for (auto i = 0; i < submeshes_.size(); i++) {
            submeshes_[i].material->bind();
            auto indices = reinterpret_cast<void *>(index_size_ * submeshes_[i].start);
            OGL_CALL(glDrawElements(GL_TRIANGLES, submeshes_[i].count(), index_type_, indices));
            submeshes_[i].material->unbind();
        }
        OGL_CALL(glBindVertexArray(0u));
    }


    void *Mesh::map_vertex_buffer() {
        OGL_CALL(glBindBuffer(GL_ARRAY_BUFFER, v_buffer_.get()));
        void *ptr;
        OGL_CALL(ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
        return ptr;
    }

    void Mesh::unmap_vertex_buffer() {
        OGL_CALL(glBindBuffer(GL_ARRAY_BUFFER, v_buffer_.get()));
        auto unmap_status = glUnmapBuffer(GL_ARRAY_BUFFER);
        xe::utils::get_and_report_error("glUnmapBuffer", "src/Engine/Mesh.cpp", __LINE__ - 1, true);
        if (!unmap_status) {
            SPDLOG_CRITICAL("Error unmapping Mesh vertex buffer");
            exit(-1);
        }
    }

    void *Mesh::map_index_buffer() {
        OGL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, i_buffer_.get()));
        void *ptr;
        OGL_CALL(ptr = glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY));
        return ptr;
    }

    void Mesh::unmap_index_buffer() {
        OGL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, i_buffer_.get()));
        auto unmap_status = glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
        xe::utils::get_and_report_error("glUnmapBuffer", "src/Engine/Mesh.cpp", __LINE__ - 1, true);
        if (!unmap_status) {
            SPDLOG_CRITICAL("Error unmapping Mesh index buffer");
            exit(-1);
        }
    }


}