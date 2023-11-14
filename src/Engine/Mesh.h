//
// Created by Piotr Białas on 25/10/2021.
//

#pragma once

#include <vector>
#include "glad/gl.h"

#include "Application/RegisteredObject.h"
#include "Material.h"

namespace xe {

    enum  AttributeType {
        POSITION = 0,
        NORMAL = 1,
        TANGENT = 2,
        TEXCOORD_0 = 3,
        TEXCOORD_1 = 4,
        COLOR_0 = 5,
        COUNT = 6
    };


    class Mesh : public RegisteredObject {
    protected:
        std::vector<GLuint> attributes_;
    public:


        Mesh(GLsizei stride, GLsizei v_buffer_size, GLenum v_buffer_hint,
             GLsizei i_buffer_size, GLenum index_type, GLenum i_buffer_hint);

        virtual ~Mesh() {};


        void load_vertices(size_t offset, size_t size, void *data);

        void load_indices(size_t offset, size_t size, void *data);

        void *map_vertex_buffer();
        void unmap_vertex_buffer();
        void *map_index_buffer();
        void unmap_index_buffer();

        void add_attribute(AttributeType attr_type, GLuint size, GLenum type, GLsizei offset) const;

        void add_primitive(GLuint start, GLuint end) {
            primitives_.emplace_back(start, end);
        }



        void add_primitive(GLuint start, GLuint end, const Material *material) {
            primitives_.emplace_back(start, end, material);
        }

        virtual void draw() const;

        struct Primitive {
            Primitive(GLuint start, GLuint end) :
                    start(start), end(end), material(xe::NullMaterial::null_material()) {}

            Primitive(GLuint start, GLuint end, const Material *material) :
                    start(start), end(end), material(material) {}

            const GLuint start;
            const GLuint end;

            GLuint count() const { return end - start; }

            const Material *material;

        };


    private:
        GLuint index_size_;
        GLuint vao_;
        GLuint v_buffer_;
        GLuint i_buffer_;
        const GLenum index_type_;
        const GLsizei stride_;

        std::vector<Primitive> primitives_;

    };


}