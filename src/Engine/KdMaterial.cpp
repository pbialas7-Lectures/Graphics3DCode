//
// Created by pbialas on 27.09.23.
//

#include "KdMaterial.h"

#include "glad/gl.h"

#include "Application/utils.h"
#include "Engine/mesh_loader.h"

namespace xe {
    void KdMaterial::init() {
        create_material_uniform_buffer(2 * sizeof(glm::vec4));
        create_program_in_project({{GL_VERTEX_SHADER,   "Kd_vs.glsl"},
                                   {GL_FRAGMENT_SHADER, "Kd_fs.glsl"}});

        map_Kd_location_ = glGetUniformLocation(program(), "map_Kd");
        if (map_Kd_location_ == -1) {
            SPDLOG_WARN("Cannot find map_Kd uniform");
        }
    }

    void KdMaterial::bind() const {
        OGL_CALL(glUseProgram(program()));
        OGL_CALL(glBindBufferBase(GL_UNIFORM_BUFFER, 0, material_uniform_buffer()));
        OGL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec4), &Kd_));
        OGL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec4), sizeof(int), &use_vertex_colors_));
        GLuint use_map_Kd = 0;
        if (texture_ != 0) {
            use_map_Kd = 1;
            OGL_CALL(glUniform1i(map_Kd_location_, 0));
            OGL_CALL(glActiveTexture(GL_TEXTURE0));
            OGL_CALL(glBindTexture(GL_TEXTURE_2D, texture_));
        }

        OGL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, 5 * sizeof(float), sizeof(int), &use_map_Kd));
    }

    void KdMaterial::unbind() const {
        OGL_CALL(glBindBufferBase(GL_UNIFORM_BUFFER, 0, 0));
        if(texture_>0) {
            OGL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
        }
    }

    Material *KdMaterial::make_color_material(const mtl_material_t &mat, std::string mtl_dir) {
        return nullptr;
    }

    static auto KdMaterial = xe::add_mat_function("KdMaterial", KdMaterial::make_color_material);



} // xe