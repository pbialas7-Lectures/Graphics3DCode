//
// Created by pbialas on 24.09.23.
//

#include "Material.h"

#include "spdlog/spdlog.h"
#include "glad/gl.h"

#include "Application/utils.h"
#include "AbstractMaterial.h"

const xe::NullMaterial *xe::null_material = new xe::NullMaterial();

