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
#include "Application/gl_handle.h"

namespace xe {
    template<class D>
    class AbstractMaterial : public Material {

    public:
        using Derived = D; // CRTP
        static GLuint program() { return program_; }

        static GLuint material_uniform_buffer() { return material_uniform_buffer_; }

        static void create_material_uniform_buffer(GLsizei size);

        static void create_program(const utils::shader_path_map_t &shader_paths);

        static void create_program_in_project(const utils::shader_path_map_t &shader_paths);

        static void create_program_in_engine(const utils::shader_path_map_t &shader_paths);

    private:
        // Deletes the program and uniform buffer shared by all materials of type D. It is registered on first
        // use, so RegisteredObject::cleanup() deletes them while the OpenGL context still exists.
        class SharedResources : public RegisteredObject {
        public:
            ~SharedResources() override {
                if (program_ != 0u)
                    gl::delete_program(std::exchange(program_, 0u));
                if (material_uniform_buffer_ != 0u)
                    gl::delete_buffer(std::exchange(material_uniform_buffer_, 0u));
                shared_resources_ = nullptr;
            }
        };

        static void register_shared_resources() {
            if (!shared_resources_)
                shared_resources_ = new SharedResources;
        }

        inline static GLuint program_ = 0u;
        inline static GLuint material_uniform_buffer_ = 0u;
        inline static SharedResources *shared_resources_ = nullptr;
    };


    template<class D>
    void xe::AbstractMaterial<D>::create_program(const utils::shader_path_map_t &shader_paths) {
        auto program = utils::create_program(shader_paths);
        if (!program) {
            SPDLOG_CRITICAL("Invalid program");
            exit(-1);
        }
        register_shared_resources();
        if (program_ != 0u)
            gl::delete_program(program_); // init() called again
        program_ = program;
    }

    template<class D>
    void xe::AbstractMaterial<D>::create_material_uniform_buffer(GLsizei size) {
        register_shared_resources();
        if (material_uniform_buffer_ != 0u)
            gl::delete_buffer(std::exchange(material_uniform_buffer_, 0u)); // init() called again
        OGL_CALL(glCreateBuffers(1, &material_uniform_buffer_));
        OGL_CALL(glNamedBufferData(material_uniform_buffer_, size, nullptr, GL_STATIC_DRAW));
    }

    template<class D>
    void xe::AbstractMaterial<D>::create_program_in_project(const utils::shader_path_map_t &shader_paths) {
        utils::shader_path_map_t shader_paths_in_project;
        for (std::pair<GLenum, std::string> shader_path: shader_paths) {
            shader_paths_in_project[shader_path.first] =
                    std::string(PROJECT_DIR) + "/shaders/" + shader_path.second;
        }
        create_program(shader_paths_in_project);
    }

    template<class D>
    void xe::AbstractMaterial<D>::create_program_in_engine(const utils::shader_path_map_t &shader_paths) {
        utils::shader_path_map_t shader_paths_in_engine;
        for (std::pair<GLenum, std::string> shader_path: shader_paths) {
            shader_paths_in_engine[shader_path.first] =
                    std::string(ROOT_DIR) + "/src/Engine/shaders/" + shader_path.second;
        }
        create_program(shader_paths_in_engine);
    }

}
