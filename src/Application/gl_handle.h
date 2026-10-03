//
// Move-only owners of OpenGL object names.
//

#pragma once

#include <utility>

#include "glad/gl.h"

#include "Application/utils.h"

namespace xe::gl {

    // Errors during deletion are reported but never abort: deleters run from destructors,
    // often during shutdown, where aborting would only hide the original problem.
#define XE_GL_DELETE(call) \
        do { call; xe::utils::get_and_report_error(#call, __FILE__, __LINE__, false); } while (0)

    inline void delete_buffer(GLuint id) { XE_GL_DELETE(glDeleteBuffers(1, &id)); }

    inline void delete_vertex_array(GLuint id) { XE_GL_DELETE(glDeleteVertexArrays(1, &id)); }

    inline void delete_texture(GLuint id) { XE_GL_DELETE(glDeleteTextures(1, &id)); }

    inline void delete_program(GLuint id) { XE_GL_DELETE(glDeleteProgram(id)); }

#undef XE_GL_DELETE

    // Owns a single OpenGL object name and deletes it when destroyed. It cannot be copied, only moved,
    // so every object has exactly one owner. Name 0 means "no object" and is never deleted.
    // The deleter is a plain function because glad's GL entry points are macros, not functions.
    // An owner must be destroyed while the OpenGL context still exists.
    template<void (*Delete)(GLuint)>
    class Handle {
    public:
        Handle() = default;

        explicit Handle(GLuint id) : id_(id) {}

        ~Handle() { reset(); }

        Handle(Handle &&other) noexcept: id_(std::exchange(other.id_, 0u)) {}

        Handle &operator=(Handle &&other) noexcept {
            if (this != &other) {
                reset();
                id_ = std::exchange(other.id_, 0u);
            }
            return *this;
        }

        Handle(const Handle &) = delete;

        Handle &operator=(const Handle &) = delete;

        GLuint get() const { return id_; }

        // Deletes the current object, if any, and returns a pointer for a glCreate*(1, ...) / glGen*(1, ...) call.
        GLuint *put() {
            reset();
            return &id_;
        }

        // Gives up ownership without deleting the object.
        GLuint release() { return std::exchange(id_, 0u); }

        void reset() {
            if (id_ != 0u)
                Delete(std::exchange(id_, 0u));
        }

        explicit operator bool() const { return id_ != 0u; }

    private:
        GLuint id_ = 0u;
    };

    using Buffer = Handle<delete_buffer>;
    using VertexArray = Handle<delete_vertex_array>;
    using Texture = Handle<delete_texture>;
    using Program = Handle<delete_program>;
}
