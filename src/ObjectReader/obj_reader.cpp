//
// Created by Piotr Bialas on 2018-12-03.
//

#include "obj_reader.h"

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_DEBUG

#include "spdlog/spdlog.h"
#include "glm/glm.hpp"

#define TINYOBJLOADER_IMPLEMENTATION // define this in only *one* .cc

#include "3rdParty/tinyobjloader/tiny_obj_loader.h"


namespace {
    xe::Triangle get_face(tinyobj::shape_t sh, size_t index_offset, const tinyobj::attrib_t &attrib) {

        xe::Triangle triangle;
        std::vector<glm::vec3> p(3);
        for (size_t v = 0; v < 3; v++) {
            // access to vertex
            tinyobj::index_t idx = sh.mesh.indices[index_offset + v];

            p[v].x = attrib.vertices[3 * idx.vertex_index + 0];
            p[v].y = attrib.vertices[3 * idx.vertex_index + 1];
            p[v].z = attrib.vertices[3 * idx.vertex_index + 2];

            triangle.position[v] = p[v];

            glm::vec2 t;

            if (idx.texcoord_index >= 0) {
                t.x = attrib.texcoords[2 * idx.texcoord_index + 0];
                t.y = attrib.texcoords[2 * idx.texcoord_index + 1];
                triangle.tex_coord[v] = t;
                triangle.has_texcoord[v] = true;
            }


            if (idx.normal_index >= 0) {
                glm::vec3 normal;
                normal.x = attrib.normals[3 * idx.normal_index + 0];
                normal.y = attrib.normals[3 * idx.normal_index + 1];
                normal.z = attrib.normals[3 * idx.normal_index + 2];
                triangle.normal[v] = normal;
                triangle.has_normals[v] = true;
            }
        }
        return triangle;
    }

    void push_sub_mesh(xe::sMesh &s_mesh, const xe::sMesh::SubMesh sub_mesh) {
        if (sub_mesh.end > sub_mesh.start) {
            SPDLOG_DEBUG("Pushing submesh {:4d} {:4d}", sub_mesh.start, sub_mesh.end);
            s_mesh.submeshes.push_back(sub_mesh);
        }
    }

    xe::sMesh::SubMesh emit_submesh(xe::sMesh &s_mesh, const xe::sMesh::SubMesh sub_mesh) {
        push_sub_mesh(s_mesh, sub_mesh);
        return xe::sMesh::SubMesh{sub_mesh.end, 0, sub_mesh.mat_idx};
    }

    int create_smesh(xe::sMesh &mesh, const tinyobj::attrib_t &attrib, const std::vector<tinyobj::shape_t> &shapes) {

        mesh.has_normals = !attrib.normals.empty();
        mesh.has_texcoords[0] = !attrib.texcoords.empty();

        int index = 0;
        int fce = 0;

        auto mat_idx = -1;
        xe::sMesh::SubMesh sub_mesh;
        sub_mesh.start = fce;
        sub_mesh.mat_idx = mat_idx;
        for (auto sh: shapes) {
            SPDLOG_DEBUG("Processing shape `{}'", sh.name);
            size_t index_offset = 0;

            for (size_t f = 0; f < sh.mesh.num_face_vertices.size(); f++) {
                if (sh.mesh.material_ids[f] != mat_idx) {
                    sub_mesh.end = fce;
                    sub_mesh = emit_submesh(mesh, sub_mesh);

                    mat_idx = sh.mesh.material_ids[f];
                    sub_mesh.mat_idx = mat_idx;
                }
                int fv = sh.mesh.num_face_vertices[f];
                if (fv != 3) {
                    spdlog::error("Reading a non triangular face");
                    return 1;
                }
                auto triangle = get_face(sh, index_offset, attrib);
                xe::sMesh::Face face;
                for (size_t v = 0; v < fv; v++) {
                    mesh.vertex_coords.push_back(triangle.position[v]);
                    if (!triangle.has_texcoord[v]) {
                        if (mesh.has_texcoords[0]) {
                            spdlog::warn("Some vertices have texcoord and some do not in OBJ file.");
                            mesh.has_texcoords[0]=false;
                        }
                    } else {
                        mesh.vertex_texcoords[0].push_back(triangle.tex_coord[v]);
                    }

                    if (!triangle.has_normals[v]) {
                        if (mesh.has_normals) {
                            spdlog::warn("Some vertices have normals and some do not in OBJ file.");
                            mesh.has_normals=false;
                        }
                    } else {
                        mesh.vertex_normals.push_back(triangle.normal[v]);
                    }
                    face.v[v] = index;
                    index++;
                }
                mesh.faces.push_back(face);
                index_offset += fv;
                fce++;
            }
            sub_mesh.end = fce;
            sub_mesh = emit_submesh(mesh, sub_mesh);

        }
        return 0;
    }


    bool read_obj(std::string name, std::string mtl_base_dir, tinyobj::attrib_t *attrib,
                  std::vector<tinyobj::shape_t> *shapes,
                  std::vector<tinyobj::material_t> *materials) {
        std::string err, warn;

        bool ret;
        if (mtl_base_dir.empty())
            ret = tinyobj::LoadObj(attrib, shapes, materials, &warn, &err, name.c_str());
        else
            ret = tinyobj::LoadObj(attrib, shapes, materials, &warn, &err, name.c_str(), mtl_base_dir.c_str());

        if (!warn.empty()) {
            spdlog::warn(warn);
        }
        if (!err.empty()) {
            spdlog::error(err);
        }
        return ret;
    }

}

namespace xe {
    xe::sMesh load_smesh_from_obj(std::string name, std::string mtl_base_dir) {
        SPDLOG_DEBUG("Loading obj file `{}'", name);
        xe::sMesh s_mesh;

        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;

        auto ret = read_obj(name, mtl_base_dir, &attrib, &shapes, &s_mesh.materials);
        if (!ret) {
            spdlog::error("Error reading obj file {} {}", name, mtl_base_dir);
            return s_mesh;
        }

        if (attrib.vertices.empty()) {
            spdlog::error("No vertices in obj file {}", name);
            return s_mesh;
        }

        create_smesh(s_mesh, attrib, shapes);

        return s_mesh;

    }
}




