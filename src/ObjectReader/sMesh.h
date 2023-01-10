//
// Created by pbialas on 14.12.2021.
//


#pragma once
#include <iostream>
#include <vector>
#include <array>

#include "glm/glm.hpp"

#include "Geometry/bounding_box.h"
#include "3rdParty/tinyobjloader/tiny_obj_loader.h"



namespace xe {

    using mtl_material_t = tinyobj::material_t;


    struct sMesh {
        static const uint32_t MAX_TEXCOORDS = 4u;

        sMesh() : has_normals(false), has_tangents(false), has_colors(false) {
            for (auto &&t: has_texcoords)
                t = false;
        };

        struct Face {
            std::array<uint16_t, 3> v;
        };


        struct SubMesh {
            int start;
            int end;
            int mat_idx;
        };


        std::vector <glm::vec3> vertex_coords;
        std::vector <glm::vec2> vertex_texcoords[MAX_TEXCOORDS];
        std::vector <glm::vec3> vertex_normals;
        std::vector <glm::vec4> vertex_tangents;
        std::vector <glm::vec4> vertex_colors;
        std::vector <Face> faces;

        std::vector <mtl_material_t> materials;
        std::vector <SubMesh> submeshes;

        xe::BoundingBox<3> bb;

        bool has_texcoords[MAX_TEXCOORDS];
        bool has_normals;
        bool has_tangents;
        bool has_colors;

    };

    sMesh* generate_normals(const sMesh& s_mesh) ;


}




