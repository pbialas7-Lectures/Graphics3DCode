# KdMaterial

In the previous assignment, we have provided an abstract layer over the vertex and index buffer manipulations. In this
assignment, we will add an abstraction of the material that will be responsible for coloring the primitives.

A material is an attribute of a submesh and describes how this submesh should be
colored. In general, material consists of a set of shaders that will do the coloring and uniforms that will provide
material properties to those shaders.

The `Material` class implemented in `Material.h` and `Material.cpp` in the `src/Engine/` directory is an abstract class
that will be a base class for all materials.
It defines a pure virtual function `bind` that has to be implemented in each derived class. This method should load the
required shader program and all necessary uniforms.

## AbstractMaterial class

The `AbstractMaterial` derives from `Material` class and is implemented in `AbstractMaterial.h`
and `AbstractMaterial.cpp` in the `src/Engine/`.
It provides several methods that help create concrete materials.

The `AbstractMaterial` class is a template because this enables the use of the
so-called [CRTP](https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern) (Curiously Recurring Template
Pattern) idiom.
This idiom allows us to make sure that each derived class will have its own copy of the static fields.

## Kd Material

Based on the `AbstractMaterial` class in this assignment,
we will implement a simple material that will color the primitives in
single color which will be an attribute of the material object.
We will call this material `KdMaterial`, `Kd` standing for diffuse reflection coefficient.
If the vertex colors are provided in the vertex buffer, the resulting color will be a
product of the vertex color and the color of the material

```glsl
vFragColor = vertex_color*Kd;
```

If the colors are not provided in the vertex buffer, the resulting color will be equal to the color of the material.

## KdMaterial class

1. Please define a new class `KdMaterial` in `KdMaterial.h` file in the `Engine` directory. This class should derive
   from `AbstractMaterial<KdMaterial>`
   ```c++
   #pragma once

   #include "glm/glm.hpp"

   #include "AbstractMaterial.h"

   namespace xe {
       class KdMaterial : public AbstractMaterial<KdMaterial> {
       };
   }
   ```
   In the `app.cpp` file of the assignment include this header with `#include "Engine/KdMaterial.h"`.

   The `KdMaterial` class should have a constant field `Kd_` of type `glm::vec4` that will
   store the color of the material. It should be initialized in the constructor of the class

   ```c++
   explicit KdMaterial(const glm::vec4 &Kd) : Kd_(Kd) {}
   ```

   which should be defined in the `KdMaterial` class body. We will add the possibility to use the vertex colors
   later.

   Put the definitions of the other methods of this class into a new `KdMaterial.cpp` file in the `Engine` directory.
   The `Engine` library collects its source files using `file(GLOB ...)` in its `CMakeLists.txt`, so after creating a
   new `.cpp` file you have to re-run CMake (e.g. `cmake ..` in the `build` directory, or reload the CMake project in
   your IDE). Otherwise, the new file will not be compiled and you will get "undefined reference" errors when linking.

### Material uniform buffer

1. Thanks to the CRTP (Curiously Recurring Template Pattern) trick, the `KdMaterial` class inherits from the
   `AbstractMaterial<KdMaterial>` class its own static `material_uniform_buffer_` variable.
   The material uniform buffer has to be created before we start using this class. This will be done in the
   static `KdMaterial::init()` method. Go ahead and declare this method in the `KdMaterial` class
   ```c++
   static void init();
   ```
   add its empty implementation and call it at the beginning of the `init` method of the `SimpleShapeApplication`
   class:
   ```c++
   xe::KdMaterial::init();
   ```

2. Add code creating the material uniform buffer to this method. Use the `create_material_uniform_buffer` method
   of `AbstractMaterial` class with
   size parameter equal to `2*sizeof(glm::vec4)`.

   ```c++
   create_material_uniform_buffer(2*sizeof(glm::vec4));
   ```
   This method will create the uniform buffer and assign its handle to the private static `material_uniform_buffer_`
   field of the `AbstractMaterial<KdMaterial>` class. In `KdMaterial` you can access it using the
   `material_uniform_buffer()` method.

### Shader program

Another attribute of the material is the shader program implementing the coloring. The handle to this program is stored
in the `program_` field, which is also static.
This field has also to be initialized in the `init` method using the `create_program_in_engine` method.

1. In preparation, move the shader source files from the `src/Assignments/11_KdMaterial/shaders` directory to the
   `src/Engine/shaders` directory (you have to create this directory first), and
   rename them `Kd_vs.glsl` and `Kd_fs.glsl`. Change the argument of `xe::utils::create_program` method call in
   the `init` method of the `SimpleShapeApplication` class to reflect those changes: the shaders are no longer in the
   assignment directory, so instead of `PROJECT_DIR` use `std::string(ROOT_DIR) + "/src/Engine/shaders/Kd_vs.glsl"`
   and similarly for the fragment shader. Delete the empty `shaders` directory.

2. Add the `create_program_in_engine` method call to the `KdMaterial::init` method.

   ```c++
   create_program_in_engine({{GL_VERTEX_SHADER, "Kd_vs.glsl"},
                              {GL_FRAGMENT_SHADER, "Kd_fs.glsl"}});
   ```

   `KdMaterial::create_program_in_engine` is just "syntactic sugar" for the call to the `xe::utils::create_program`. It
   allows specifying only the shader file names, and the method will automatically add the path to the `Engine/shaders`
   directory. Please define the `KdMaterial::init` method in the `KdMaterial.cpp` file.

3. Now please define the `bind` method of the `KdMaterial` class that will just load the shader program using
   the `glUseProgram` function. Add the declaration in the `KdMaterial.h` file in the body of `KdMaterial` class:
   ```c++
   void bind() const override;
   ```
   and add its definition to the `KdMaterial.cpp` file:
   ```c++
   void KdMaterial::bind() const {
       OGL_CALL(glUseProgram(program()));
   }
   ```

4. Create an object of the `KdMaterial` class in the `init` method of the `SimpleShapeApplication` class

   ```c++
   auto kd_white_material = new xe::KdMaterial(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
   ```
   and pass it as an argument to the `add_submesh` method of the `Mesh` class.

   ```c++
   pyramid->add_submesh(0, indices.size(), kd_white_material);
   ```

   Like meshes, materials are derived from `RegisteredObject`, so they have to be created with `new` and are deleted
   automatically when the application finishes; do not delete them yourself.

5. Delete the `glUseProgram` call from the `SimpleShapeApplication::init` method.

6. Delete the code creating the shader program from the `init` method of the `SimpleShapeApplication` class.

At this point, you should have the program running and displaying the pyramid as before. But we are not yet using
the color from the KdMaterial. To do this, we have to modify the fragment shader.

1. In the fragment shader add the uniform interface block.
   This block will contain the `Kd` uniform variable and a boolean variable
   that will indicate if the vertex colors should be used.
   Please note that setting this variable to `true` assumes that the vertex colors are present in the vertex buffer and
   the appropriate attribute is enabled.

   ```glsl
   layout(std140, binding=0) uniform KdMaterial {
       vec4 Kd;
       bool use_vertex_colors;
   };
   ```
   The binding point 0 is free: the `Transformations` block uses binding point 1, and the `Mixer` block that used 0 was
   removed in the `Pyramid` assignment.

2. To the `bind` method of the `KdMaterial` class add the call that will load the `Kd` material uniform buffer

   ```c++
   OGL_CALL(glBindBufferBase(GL_UNIFORM_BUFFER, 0, material_uniform_buffer()));
   OGL_CALL(glNamedBufferSubData(material_uniform_buffer(), 0, sizeof(glm::vec4), &Kd_));
   OGL_CALL(glNamedBufferSubData(material_uniform_buffer(), sizeof(glm::vec4), sizeof(int), &use_vertex_colors_));
   ```

   The `use_vertex_colors_` is an integer field of the `KdMaterial` class
   that should be set to `1` if the vertex colors are present in the vertex buffer and are to be used, and `0`
   otherwise. It has to be an `int` and not a `bool`: the second `glNamedBufferSubData` call above copies `sizeof(int)` bytes from
   it, as a `bool` in a `std140` uniform block takes four bytes, while a C++ `bool` usually takes only one.
   Add this field to the class, and replace the existing constructor with these two:

   ```c++
   KdMaterial(const glm::vec4 &Kd, bool use_vertex_colors) : Kd_(Kd), use_vertex_colors_(use_vertex_colors) {}
   explicit KdMaterial(const glm::vec4 &Kd) : KdMaterial(Kd, false) {}
   ```

3. Add an `unbind` method that unbinds the material uniform buffer. Declare it in the class body
   ```c++
   void unbind() const override;
   ```
   and in its definition in `KdMaterial.cpp` call `glBindBufferBase(GL_UNIFORM_BUFFER, 0, 0)` (wrapped in `OGL_CALL`).
   `Mesh::draw` calls `unbind` after drawing each submesh.

4. Assign the `Kd` variable to the `vFragColor` variable in the fragment shader.

   ```glsl
   vFragColor = Kd;
   ```

   You should now see the pyramid in white color.

5. Assign the product of `Kd` variable and vertex colors to the `vFragColor` variable in the fragment shader

   ```glsl
   vFragColor = vertex_color*Kd;
   ```

   You should now see the pyramid in the color of the vertex colors.

6. Add the code that uses the vertex colors or not, depending on the value of the `use_vertex_colors` variable.

   You should now see the pyramid in white color, as the default value of this attribute is `false`.
   Create the material using the two-parameter constructor with `true` as the second argument, and you should see the
   pyramid in the color of the vertex colors.
   Change it back to `false`.

7. Divide the pyramid into five submeshes corresponding to the faces and the base.
   Assign a different color to each of the submeshes, creating a separate `KdMaterial` with this color and
   `use_vertex_colors` set to `false` for each of them. Keep the same colors as before. It is best to add the submeshes
   one by one.
