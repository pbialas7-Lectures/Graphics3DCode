# Reading Wavefront OBJ files

In this assignment, we will add the possibility of loading the models from files
in [Wavefront OBJ](https://paulbourke.net/dataformats/obj/) format and
associated [Wavefront Material Template Library (MTL)](https://paulbourke.net/dataformats/mtl/) files.

## Loading textures

1. Add a new header file `texture.h` in the `Engine` directory and declare `create_texture` function in the `xe`
   namespace:
   ```c++
   #pragma once

   #include <string>

   #include "glad/gl.h"

   namespace xe {
       GLuint create_texture(const std::string &name, bool is_sRGB = true);
   }
   ```

2. Add the definition of this function in the `texture.cpp` file. This function should take the name of the texture file
   and return the OpenGL handle to the texture.
   Use the code creating the texture that was previously in the `app.cpp` file; `texture.cpp` has to include the
   `stb/stb_image.h` header. As before, choose the formats
   depending on the number of channels of the image: the format of the data is `GL_RGB` for three and `GL_RGBA` for
   four channels. If `is_sRGB` is `true`, the internal format should be `GL_SRGB8` or `GL_SRGB8_ALPHA8`, otherwise
   `GL_RGB8` or `GL_RGBA8`. Remember to free the image with `stbi_image_free` after loading it into the texture.

   If the image cannot be loaded, do not exit the program as before, but print an error message and return zero.
   Zero is never a valid texture handle, so the callers can check for it (see the `create_from_mtl` code in the next
   section).

   The `Engine` library collects its source files using `file(GLOB ...)`, so after creating `texture.cpp` you have to
   re-run CMake, as you did after creating `KdMaterial.cpp`.

3. In the `app.cpp` file, use this newly defined function to load the texture.

## Materials from MTL files

1. In class `KdMaterial` add a new _factory_ method that will create a material object from the MTL description. This
   method must be `static`:
   ```c++
   static Material *create_from_mtl(const mtl_material_t &mat, std::string mtl_dir);
   ```
   You will need to include `ObjectReader/sMesh.h` file where the `mtl_material_t` is defined.
   Also add a `void set_texture(GLuint texture)` method to the `KdMaterial` class that sets the `texture_` field.

2. Add the definition of this function
   ```c++
   Material *KdMaterial::create_from_mtl(const mtl_material_t &mat, std::string mtl_dir) {
       glm::vec4 color = get_color(mat.diffuse);
       color = glm::vec4(xe::srgb_inverse_gamma_correction(glm::vec3(color)), color.a);
       SPDLOG_DEBUG("Adding KdMaterial {}", glm::to_string(color));
       auto material = new xe::KdMaterial(color);
       if (!mat.diffuse_texname.empty()) {
           auto texture = xe::create_texture(mtl_dir + "/" + mat.diffuse_texname, true);
           SPDLOG_DEBUG("Adding Texture {} {:1d}", mat.diffuse_texname, texture);
           if (texture > 0) {
               material->set_texture(texture);
           }
       }

       return material;
   }
   ```
   The `get_color` and `srgb_inverse_gamma_correction` functions are declared in `Engine/utils.h` and `create_texture`
   in `Engine/texture.h`, so include both headers in `KdMaterial.cpp`.
   `glm::to_string` requires defining `GLM_ENABLE_EXPERIMENTAL` before including the `glm/gtx/string_cast.hpp` header.

   This function assumes that the textures are in sRGB space, and so are the colors in MTL files, which are usually
   picked in a color picker. Our shader works with linear colors and gamma-corrects the result, so the `Kd` color has to
   be converted to linear space using `srgb_inverse_gamma_correction`; otherwise it would be gamma-corrected twice and
   look too light.

   In the `KdMaterial::init` function add the following code (the `add_mat_function` function is declared in
   `Engine/mesh_loader.h`, so include it in `KdMaterial.cpp`):
   ```c++
   xe::add_mat_function("KdMaterial", KdMaterial::create_from_mtl);
   ```
   that will register this function as a factory method for the `KdMaterial` class. The OBJ loader uses the registered
   factories to create the materials, so `KdMaterial::init()` has to be called __before__ loading any meshes.

3. Replace all the code creating the pyramid mesh and material by
   ```c++
   auto pyramid = xe::load_mesh_from_obj(std::string(ROOT_DIR) + "/Models/pyramid.obj",
                                         std::string(ROOT_DIR) + "/Models");
   if (!pyramid) {
       SPDLOG_CRITICAL("Cannot load the pyramid model");
       exit(-1);
   }
   add_mesh(pyramid);
   ```
   The `load_mesh_from_obj` function is declared in `Engine/mesh_loader.h`, include it in `app.cpp`. It returns
   `nullptr` when the model cannot be read, e.g. because of a wrong path, hence the check.
   You should again see the textured pyramid.

4. Finally, load the `Models/blue_marble.obj` model instead of the pyramid.
   You should see the Earth model with the texture.

   The loader does not share vertices between triangles, so each triangle adds three vertices, and it stores the
   indices as 16-bit numbers. So it can only load models with at most 65536 vertices, i.e. 21845 triangles, and reports
   an error for larger ones. This is enough for the models used in this course.
