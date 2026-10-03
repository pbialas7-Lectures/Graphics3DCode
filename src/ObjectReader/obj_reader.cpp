//
// Created by Piotr Bialas on 2018-12-03.
//

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_DEBUG

#include "obj_reader.h"

#include <filesystem>
#include <limits>
#include <vector>

#include "spdlog/spdlog.h"
#include "glm/glm.hpp"

#include "3rdParty/MIKKTSpace/mikktspace.h"

#define TINYOBJLOADER_IMPLEMENTATION // define this in only *one*  source file
#define TINYOBJLOADER_USE_MAPBOX_EARCUT

#include "3rdParty/tinyobjloader/tiny_obj_loader.h"


namespace {
    // A single triangle as read from the OBJ file, with the attributes of its three vertices.
    struct Triangle {
        glm::vec3 position[3];
        glm::vec2 tex_coord[3];
        glm::vec3 normal[3];

        bool has_normals[3] = {false, false, false};
        bool has_texcoord[3] = {false, false, false};
    };

    // The shape is passed by reference: copying it would copy all its indices for every face.
    Triangle get_face(const tinyobj::shape_t &sh, size_t index_offset, const tinyobj::attrib_t &attrib) {

        Triangle triangle;
        for (size_t v = 0; v < 3; v++) {
            // access to vertex
            tinyobj::index_t idx = sh.mesh.indices[index_offset + v];

            glm::vec3 p;
            p.x = attrib.vertices[3 * idx.vertex_index + 0];
            p.y = attrib.vertices[3 * idx.vertex_index + 1];
            p.z = attrib.vertices[3 * idx.vertex_index + 2];

            triangle.position[v] = p;

            if (idx.texcoord_index >= 0) {
                glm::vec2 t;
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
        return xe::sMesh::SubMesh{sub_mesh.end, sub_mesh.end, sub_mesh.mat_idx};
    }

    int create_smesh(xe::sMesh &mesh, const tinyobj::attrib_t &attrib, const std::vector<tinyobj::shape_t> &shapes) {

        // Vertices are not shared between faces, so each face adds three vertices, and they are indexed with 16-bit
        // indices (sMesh::Face). Larger meshes would silently get wrong indices.
        size_t n_vertices = 0;
        for (const auto &sh: shapes)
            n_vertices += sh.mesh.indices.size();
        const size_t max_vertices = std::numeric_limits<uint16_t>::max() + size_t(1);
        if (n_vertices > max_vertices) {
            SPDLOG_ERROR("OBJ mesh needs {} vertices, but at most {} ({} triangles) can be indexed with 16-bit indices",
                         n_vertices, max_vertices, max_vertices / 3);
            return 1;
        }

        mesh.has_normals = !attrib.normals.empty();
        mesh.has_texcoords[0] = !attrib.texcoords.empty();

        int index = 0;
        int fce = 0;

        auto mat_idx = -1;
        xe::sMesh::SubMesh sub_mesh;
        sub_mesh.start = fce;
        sub_mesh.mat_idx = mat_idx;
        for (const auto &sh: shapes) {
            SPDLOG_DEBUG("Processing shape `{}'", sh.name);
            size_t index_offset = 0;

            for (size_t f = 0; f < sh.mesh.num_face_vertices.size(); f++) {
                if (sh.mesh.material_ids[f] != mat_idx) {
                    sub_mesh.end = fce;
                    sub_mesh = emit_submesh(mesh, sub_mesh);
                    SPDLOG_DEBUG("New material {:4d} {:4d}", fce, sh.mesh.material_ids[f]);

                    mat_idx = sh.mesh.material_ids[f];
                    sub_mesh.mat_idx = mat_idx;
                }
                int fv = sh.mesh.num_face_vertices[f];
                if (fv != 3) {
                    SPDLOG_ERROR("Reading a non triangular face");
                    return 1;
                }
                auto triangle = get_face(sh, index_offset, attrib);
                xe::sMesh::Face face;
                for (size_t v = 0; v < fv; v++) {
                    mesh.vertex_coords.push_back(triangle.position[v]);
                    if (!triangle.has_texcoord[v]) {
                        if (mesh.has_texcoords[0]) {
                            SPDLOG_WARN("Some vertices have texture coordinates and some do not in OBJ file.");
                            mesh.has_texcoords[0] = false;
                        }
                    } else {
                        mesh.vertex_texcoords[0].push_back(triangle.tex_coord[v]);
                    }

                    if (!triangle.has_normals[v]) {
                        if (mesh.has_normals) {
                            SPDLOG_WARN("Some vertices have normals and some do not in OBJ file.");
                            mesh.has_normals = false;
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
        }
        // Consecutive faces with the same material form one submesh, even if they belong to different shapes.
        sub_mesh.end = fce;
        emit_submesh(mesh, sub_mesh);
        return 0;
    }

    // Computes the tangents of the mesh with MikkTSpace, the convention used by the tools that create normal maps.
    // Requires normals and texture coordinates. The vertices of the mesh are not shared between faces (vertex
    // v of face f has index 3 * f + v), which is exactly what MikkTSpace expects.
    bool generate_tangents(xe::sMesh &mesh) {
        SMikkTSpaceInterface iface{};
        iface.m_getNumFaces = [](const SMikkTSpaceContext *context) -> int {
            return static_cast<int>(static_cast<xe::sMesh *>(context->m_pUserData)->faces.size());
        };
        iface.m_getNumVerticesOfFace = [](const SMikkTSpaceContext *, const int) -> int { return 3; };
        iface.m_getPosition = [](const SMikkTSpaceContext *context, float out[], const int face, const int vert) {
            auto mesh = static_cast<xe::sMesh *>(context->m_pUserData);
            auto p = mesh->vertex_coords[mesh->faces[face].v[vert]];
            out[0] = p.x;
            out[1] = p.y;
            out[2] = p.z;
        };
        iface.m_getNormal = [](const SMikkTSpaceContext *context, float out[], const int face, const int vert) {
            auto mesh = static_cast<xe::sMesh *>(context->m_pUserData);
            auto n = mesh->vertex_normals[mesh->faces[face].v[vert]];
            out[0] = n.x;
            out[1] = n.y;
            out[2] = n.z;
        };
        iface.m_getTexCoord = [](const SMikkTSpaceContext *context, float out[], const int face, const int vert) {
            auto mesh = static_cast<xe::sMesh *>(context->m_pUserData);
            auto t = mesh->vertex_texcoords[0][mesh->faces[face].v[vert]];
            out[0] = t.x;
            out[1] = t.y;
        };
        // The sign gives the orientation of the bitangent: bitangent = sign * cross(normal, tangent).
        iface.m_setTSpaceBasic = [](const SMikkTSpaceContext *context, const float tangent[], const float sign,
                                    const int face, const int vert) {
            auto mesh = static_cast<xe::sMesh *>(context->m_pUserData);
            mesh->vertex_tangents[mesh->faces[face].v[vert]] = glm::vec4(tangent[0], tangent[1], tangent[2], sign);
        };

        SMikkTSpaceContext context{};
        context.m_pInterface = &iface;
        context.m_pUserData = &mesh;

        mesh.vertex_tangents.assign(mesh.vertex_coords.size(), glm::vec4(0.0f));
        if (!genTangSpaceDefault(&context)) {
            mesh.vertex_tangents.clear();
            return false;
        }
        return true;
    }

    tinyobj::ObjReader parse_obj(const std::string &name, const std::string &mtl_base_dir) {
        tinyobj::ObjReaderConfig reader_config;

        // By default look for the MTL files next to the OBJ file.
        if (mtl_base_dir.empty()) {
            auto obj_dir = std::filesystem::path(name).parent_path();
            reader_config.mtl_search_path = obj_dir.empty() ? "./" : obj_dir.string();
        } else
            reader_config.mtl_search_path = mtl_base_dir;

        tinyobj::ObjReader reader;

        if (!reader.ParseFromFile(name, reader_config)) {
            SPDLOG_ERROR("Error parsing OBJ file {} : {}", name, reader.Error());
        }

        if (!reader.Warning().empty()) {
            SPDLOG_WARN("Warning parsing OBJ file {} : {}", name, reader.Warning());
        }
        return reader;
    }
}

namespace xe {
    xe::sMesh load_smesh_from_obj(const std::string &name, const std::string &mtl_base_dir) {
        SPDLOG_DEBUG("Loading OBJ file `{}'", name);
        xe::sMesh s_mesh;

        auto reader = parse_obj(name, mtl_base_dir);
        if (!reader.Valid()) {
            SPDLOG_ERROR("Error reading OBJ file {} {}", name, mtl_base_dir);
            return s_mesh;
        }

        auto &attrib = reader.GetAttrib();
        auto &shapes = reader.GetShapes();
        s_mesh.materials = reader.GetMaterials();

        if (attrib.vertices.empty()) {
            SPDLOG_ERROR("No vertices in OBJ file {}", name);
            return s_mesh;
        }

        if (create_smesh(s_mesh, attrib, shapes) != 0) {
            SPDLOG_ERROR("Error reading OBJ file {}", name);
            return xe::sMesh{};
        }

        // Tangents are needed for normal mapping, which uses both the normals and the texture coordinates.
        if (s_mesh.has_normals && s_mesh.has_texcoords[0]) {
            s_mesh.has_tangents = generate_tangents(s_mesh);
            if (!s_mesh.has_tangents)
                SPDLOG_WARN("Could not generate tangents for OBJ file {}", name);
        }

        return s_mesh;
    }

}
