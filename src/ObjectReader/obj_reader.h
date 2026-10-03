//
// Created by Piotr Bialas on 2018-12-03.
//

#pragma once

#include <string>

#include "sMesh.h"

namespace xe {

    // Reads a Wavefront OBJ file and the MTL files it refers to. The MTL files are looked for in mtl_base_dir, or next
    // to the OBJ file if mtl_base_dir is empty. Returns an empty mesh (no vertices) on error.
    xe::sMesh load_smesh_from_obj(const std::string &name, const std::string &mtl_base_dir);
}
