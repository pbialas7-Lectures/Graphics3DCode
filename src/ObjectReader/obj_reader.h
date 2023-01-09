//
// Created by Piotr Bialas on 2018-12-03.
//

#pragma once

#include <iostream>
#include <vector>
#include <array>

#include "glm/glm.hpp"

#include "Geometry/bounding_box.h"
#include "3rdParty/tinyobjloader/tiny_obj_loader.h"
#include "sMesh.h"

namespace xe {

    struct Triangle {
        Triangle() : has_normals{false, false, false},
                     has_texcoord{false, false, false} {};

        glm::vec3 position[3];
        glm::vec2 tex_coord[3];
        glm::vec3 normal[3];

        bool has_normals[3];
        bool has_texcoord[3];
    };


    xe::sMesh load_smesh_from_obj(std::string name, std::string mtl_base_dir);
}

