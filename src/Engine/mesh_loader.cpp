//
// Created by Piotr Białas on 04/11/2021.
//




#include "mesh_loader.h"


#include <memory>


#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_DEBUG

#include "spdlog/spdlog.h"

#include "glm/gtx/string_cast.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "ObjectReader/obj_reader.h"
#include "Engine/Material.h"
#include "Engine/Mesh.h"


namespace xe {

    static std::unordered_map<std::string, mat_function_t> mat_functions = {{}};

    using uint = unsigned int;

    Mesh *load_mesh_from_obj(std::string path, std::string mtl_dir) {

        auto smesh = xe::load_smesh_from_obj(path, mtl_dir);
        if (smesh.vertex_coords.empty())
            return nullptr;


        uint n_floats_per_vertex = 3;
        for (auto &&t: smesh.has_texcoords) {
            if (t)
                n_floats_per_vertex += 2;
        }
        if (smesh.has_normals)
            n_floats_per_vertex += 3;
        if (smesh.has_tangents)
            n_floats_per_vertex += 4;


        size_t stride = n_floats_per_vertex * sizeof(GLfloat);

        auto n_vertices = smesh.vertex_coords.size();
        auto n_indices = 3 * smesh.faces.size();

        size_t vertex_buffer_size = smesh.vertex_coords.size() * stride;
        size_t index_buffer_size = smesh.faces.size() * 3 * sizeof(uint16_t);

        auto mesh = new Mesh(stride, vertex_buffer_size, GL_STATIC_DRAW, index_buffer_size, GL_UNSIGNED_SHORT,
                             GL_STATIC_DRAW);


        mesh->load_indices(0, n_indices * sizeof(uint16_t), smesh.faces.data());
        mesh->add_attribute(xe::AttributeType::POSITION, 3, GL_FLOAT, 0);


        auto v_ptr = reinterpret_cast<uint8_t *>(mesh->map_vertex_buffer());

        size_t offset = 0;

        {

            auto v_offset = offset;
            for (auto i = 0; i < smesh.vertex_coords.size(); i++, v_offset += stride) {
                SPDLOG_DEBUG("vertex[{}] {} ", i, glm::to_string(smesh.vertex_coords[i]));
                std::memcpy(v_ptr + v_offset, glm::value_ptr(smesh.vertex_coords[i]), sizeof(glm::vec3));
            }
        }
        offset += 3 * sizeof(GLfloat);

        for (int it = 0; it < xe::sMesh::MAX_TEXCOORDS; it++) {
            if (smesh.has_texcoords[it]) {
                mesh->add_attribute(static_cast<xe::AttributeType>(xe::AttributeType::TEXCOORD_0 + it), 2, GL_FLOAT,
                                    offset);

                auto v_offset = offset;
                for (auto i = 0; i < smesh.vertex_texcoords[it].size(); i++, v_offset += stride) {
                    SPDLOG_DEBUG("texcoord[{}] {} ", i, glm::to_string(smesh.vertex_texcoords[0][i]));
                    std::memcpy(v_ptr + v_offset, glm::value_ptr(smesh.vertex_texcoords[0][i]), sizeof(glm::vec2));
                }
                offset += 2 * sizeof(GLfloat);
            }
        }
        if (smesh.has_normals) {

            mesh->add_attribute(xe::AttributeType::NORMAL, 3, GL_FLOAT, offset);

            auto v_offset = offset;
            for (auto i = 0; i < smesh.vertex_normals.size(); i++, v_offset += stride) {
                SPDLOG_DEBUG("normal[{}] {} ", i, glm::to_string(smesh.vertex_normals[i]));
                std::memcpy(v_ptr + v_offset, glm::value_ptr(smesh.vertex_normals[i]), sizeof(glm::vec3));
            }

            offset += 3 * sizeof(GLfloat);
        }

        if (smesh.has_tangents) {
            mesh->add_attribute(xe::AttributeType::TANGENT, 4, GL_FLOAT, stride);
            offset += 4 * sizeof(GLfloat);
        }

        mesh->unmap_vertex_buffer();


        for (int i = 0; i < smesh.submeshes.size(); i++) {
            auto sm = smesh.submeshes[i];
            SPDLOG_DEBUG("Adding submesh {:4d} {:4d} {:4d}", i, sm.start, sm.end);
            Material *material = (Material *) (xe::null_material);
            if (sm.mat_idx >= 0) {
                auto mat = smesh.materials[sm.mat_idx];
                switch (mat.illum) {
                    case 0:
                        material = mat_functions["KdMaterial"](mat, mtl_dir);
                        break;
                    case 1:
                        material = mat_functions["PhongMaterial"](mat, mtl_dir);
                        break;
                }
                mesh->add_primitive(3 * sm.start, 3 * sm.end, material);
            }
        }

        return mesh;
    }

    mat_function_t add_mat_function(std::string name, mat_function_t func) {
        mat_functions[name] = func;
        SPDLOG_INFO(mat_functions.empty());
        return func;
    }
    }

#if 0
namespace {

    xe::Material *make_color_material(const xe::mtl_material_t &mat, std::string mtl_dir) {

        glm::vec4 color;
        for (int i = 0; i < 3; i++)
            color[i] = mat.diffuse[i];
        color[3] = 1.0;
        SPDLOG_DEBUG("Adding ColorMaterial {}", glm::to_string(color));
        auto material = new xe::KdMaterial(color);
        if (!mat.diffuse_texname.empty()) {
            auto texture = xe::create_texture(mtl_dir + "/" + mat.diffuse_texname);
            SPDLOG_DEBUG("Adding Texture {} {:1d}", mat.diffuse_texname, texture);
            if (texture > 0) {
                material->set_texture(texture);
            }
        }

        return material;
    }

    glm::vec4 get_color(const float c[3]) {
        glm::vec4 color;
        for (int i = 0; i < 3; i++)
            color[i] = c[i];
        color[3] = 1.0;
        return color;
    }

}
#endif