//
// Created by Piotr Białas on 04/11/2021.
//

#pragma once

#include <string>
#include <memory>
#include <unordered_map>
#include "ObjectReader/sMesh.h"

#include "Engine/Material.h"
#include "Engine/Mesh.h"

namespace xe {

    using mat_function_t = std::add_pointer<xe::Material *(const xe::mtl_material_t &mat, std::string mtl_dir)>::type;

    Mesh *load_mesh_from_obj(std::string path, std::string mtl_dir);


    mat_function_t add_mat_function(std::string name, mat_function_t func);
}
