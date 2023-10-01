//
// Created by pbialas on 27.09.23.
//

#pragma once

#include <glad/gl.h>
#include <glm/glm.hpp>
#include "spdlog/spdlog.h"


#include "Material.h"
#include "Application/utils.h"
#include "Application/RegisteredObject.h"

namespace xe {
    template<class D>
    class AbstractMaterial : public Material {

    public:
        using Derived = D; // CRTP
        static GLuint program() { return program_; }

        static GLuint material_uniform_buffer() { return material_uniform_buffer_; }

        static const GLuint material_uniform_buffer_binding_point = 0;

        static void create_material_uniform_buffer(GLsizei size);

        static void create_program(const utils::shader_source_map_t &shader_sources);

        static void create_program_in_project(const utils::shader_source_map_t &shader_sources);

    private:
        inline static GLuint program_ = 0u;
        inline static GLuint material_uniform_buffer_ = 0u;
    };


    template<class D>
    void xe::AbstractMaterial<D>::create_program(const utils::shader_source_map_t &shader_sources) {
        auto program = utils::create_program(shader_sources);
        if (!program) {
            SPDLOG_CRITICAL("Invalid program");
            exit(-1);
        }
        program_ = program;
    }

    template<class D>
    void xe::AbstractMaterial<D>::create_material_uniform_buffer(GLsizei size) {
        glGenBuffers(1, &material_uniform_buffer_);

        glBindBuffer(GL_UNIFORM_BUFFER, material_uniform_buffer_);
        glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_STATIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0u);
    }

    template<class D>
    void xe::AbstractMaterial<D>::create_program_in_project(const utils::shader_source_map_t &shader_sources) {
        utils::shader_source_map_t shader_sources_in_project;
        for (std::pair<GLenum, std::string> shader_source: shader_sources) {
            shader_sources_in_project[shader_source.first] = std::string(PROJECT_DIR) + "/shaders/" + shader_source.second;
        }
        create_program(shader_sources_in_project);
    }

}
