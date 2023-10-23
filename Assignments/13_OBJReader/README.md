# Reading Wavefront OBJ files

In this assignment, we will add the possibility of loading the models from files
in [Wavefront OBJ](http://paulbourke.net/dataformats/obj/) format and
associated [Wavefront Material Template Library (MTL)](http://paulbourke.net/dataformats/mtl/) files.

1. Add a new header file `texture.h` in the `Engine` directory and declare `create_texture` function in the `xe`
   namespace:
    ```c++
    namespace xe {
      GLuint create_texture(const std::string &name, bool is_sRGB = true);
    }
    ```

2. Add the definition of this function in the `texture.cpp` file. This function should take the name of the texture file
   and return the OpenGL handle to the texture. If the `is_sRGB`
   is `true`,
   function should use the `GLSRGB` internal format while loading the texture. Otherwise, it should use `GL_RGB`
   internal format.
   Use the code creating the texture that was previously in the `app.cpp` file.

3. In the `app.cpp` file, use this newly defined function to load the texture.


1. In class `KdMaterial` add a new _factory_ method that will create a material object from the MTL description. This
   method must be `static`:
    ```c++
    static Material *create_from_mtl(const mtl_material_t &mat, std::string mtl_dir);
    ```
   You will need to include `ObjReader/sMesh.h` file where the `mtl_material_t` is defined.

2. Add the definition of this function
   ```c++
   Material *KdMaterial::create_from_mtl(const mtl_material_t &mat, std::string mtl_dir) {
        glm::vec4 color = get_color(mat.diffuse);
        SPDLOG_DEBUG("Adding ColorMaterial {}", glm::to_string(color));
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
   This function assumes that the textures are in sRGB space. 
   In the `KdMaterial::init` function add the following code:
   ```c++
   xe::add_mat_function("KdMaterial", KdMaterial::create_from_mtl);
   ```
   that will register this function as a factory method for the `KdMaterial` class.

3. Replace all the code creating pyramid mesh and material by
   ```c++
    auto pyramid = load_mesh_from_obj("Models/pyramid.obj", "Models");
    add_mesh(pyramid);
   ```
   You should again see the textured pyramid.

4. Finally, load the `Models/blue_marble.obj` model instead of the pyramid.
   You should see the Earth model with the
   texture.  