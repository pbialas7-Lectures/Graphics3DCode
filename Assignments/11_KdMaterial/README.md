# Materials

In the previous assignment, we have provided an abstract layer over the vertex and index buffer manipulations. In this
assignment, we will add an abstraction of the material that will be responsible for coloring the primitives.

A material is an attribute on the primitive and describes how this primitive should be
colored. In general, material consists of a set of shaders that will do the coloring and uniforms that will provide
material properties to those shaders.

The `Material` class implemented in `Material.h` and `Material.cpp` in the `src/Engine/` directory is an abstract class
that will be a base class for all materials.
It defines a pure virtual function `bind` that has to be implemented in each derived class. This method should load the
required shader program and all necessary uniforms.

# AbstractMaterial class

The `AbstractMaterial`  derives from `Material` class and is implemented in `AbstractMaterial.h`
and `AbstractMaterial.cpp` in the `src/Engine/`.
It provides several methods that help create concrete materials.

The `AbstractMaterial` class is a template because this enables of the use of the
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
   namespace xe {
       class KdMaterial : public AbstractMaterial<KdMaterial> {
       }
    }
   ```

   The `KdMaterial` class should have a constant field `Kd_` of type `glm::vec4` that will
   store the color of the material. This field should be initialized in the constructor of the class.

   ```c++
      KdMaterial(const glm::vec4 &Kd) : Kd_(Kd) {}
   ```

   which should be defined in the  `KdMaterial` class body.

### Material uniform buffer

1. Thanks to the CRTP (Curiously Reoccurring Template Pattern) trick,
   class `KdMaterial` will inherit from `AbstractMaterial<KdMaterial>` class,
   its own `material_uniform_buffer_` static variable.
   The material uniform buffer has to be created before we start using this class. This will be done in the
   static `KDMaterial::init()` method.
   Go ahead and add this method and its empty implementation to the `KdMaterial` class and call this method at the
   beginning of the `init` method of `SimpleShapeApplication` class.

2. Add code creating the material uniform buffer to this method. Use the `create_material_uniform_buffer` method
   of `AbstractMaterial` class with
   size parameter equal to `2*sizeof(gm::vec4)`.

   ```c++
   create_material_uniform_buffer(2*sizeof(glm::vec4));
   ```
   This method will create the uniform buffer and assign its handle to the `material_uniform_buffer_` field of
   the `KdMaterial` class.

### Shader program

Another attribute of the material is the shader program implementing the coloring. The handle to this program is stored
in the again static `program_` field.
This field has also to be initialized in the `init` method using the `create_program_in_project` method.

1. In preparation, move the shader source files from the `11_KdMaterial/shaders` directory to `Engine/shaders`, you have
   to create this director first, and
   rename them `Kd_vs.glsl` and `Kd_fs.glsl`. Change the argument of `xe::utils::create_program` method call in
   the `init` method of the `SimpleShapeApplicationClass` to reflect those changes. Delete the empty `shaders`
   directory.

2. Add the `create_program_in_project` method call to the `KdMaterial::init` method.

   ```c++
   create_program_in_engine({{GL_VERTEX_SHADER, "Kd_vs.glsl"},
                              {GL_FRAGMENT_SHADER, "Kd_fs.glsl"}});
   ```

   `KdMaterial::create_program_in_project` is just "syntactic sugar" for the call to the `xe::utils::create_program`. It
   allows specifying only the shader file names, and the method will automatically add the path to the `Engine/shaders`
   directory. But for this to work the `KdMaterial::init` method has to be defined in the `KdMaterial.cpp` file and not
   inlined in the `KdMaterial.h` file.

3. Now please define the `bind` method of the `KdMaterial` class that will just load the shader program using
   the `glUseProgram` function. Add the declaration in the `KdMaterial.h` file in the body of `KdMaterial` class:
   ```c++
    void bind() const override;
   ``` 
   and definition in  `KdMaterial.cpp` file add the following code:
   ```c++
    void KdMaterial::KdMaterial::bind() {
         glUseProgram(program());
    }
   ``` 

4. Now we have to make sure that the `KdMaterial` class is initialized before the first use.
   To this end, we will add
   the `KdMaterial::init` method call to the `SimpleShapeApplication::init` method.

   ```c++
   KdMaterial::init();
   ```   

5. Create an object of the `KdMaterial` class in the `init` method of the `SimpleShapeApplication` class

   ```c++
   auto kd_white_material = new KdMaterial(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
   ```
   and pass it as an argument to the `add_primitive` method of the `Mesh` class.

   ```c++
    pyramid->add_primitive(0, 18, kd_white_material);
   ```

6. Delete the `glUseProgram` call from the `SimpleShapeApplication::init` method.

7. Delete the code creating the shader program from the `init` method of the `SimpleShapeApplication` class.

At this point, you should have program running and displaying the pyramid as before. But we are not yet using
the color from the KdMaterial. To do this, we have to modify the fragment shader.

1. In the fragment shader add the uniform interface block.
   This block will contain the `Kd` uniform variable and a boolean variable
   that will indicate if the vertex colors should be used.
   Please note that setting this variable to `true` assumes that the vertex colors are present in the vertex buffer and
   the appropriate attribute is enabled.

   ```glsl
   layout(std140, binding=0) uniform KdMaterial {
       vec4 Kd;
       bool use_vertex_color; 
   };
   ```
   Make sure that this binding (0) does not conflict with the biding of the PVM uniform block if so please use another
   binding.

    2. To the `bind` method of the `KdMaterial` class add the call that will load the `Kd` material uniform buffer

       ```c++
       OGL_CALL(glBindBufferBase(GL_UNIFORM_BUFFER, 0 , material_uniform_buffer_));
       OGL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec4), &Kd_));
       OGL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec4), sizeof(int), &use_vertex_colors_));
       ```

       The `use_vertex_colors_` is a integer field of the `KdMaterial` class
       that should be set to `1` if the vertex colors are present in the vertex buffer and are to be used, and `0`
       otherwise.

3. Add an `unbind` function that unbinds the material uniform buffer.

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

6. Add the code that uses or not the vertex colors depending on the value of the `use_vertex_colors` variable.

   You should now see the pyramid in white color, as the default value of this attribute is `false`.
   Change that to `true` in the constructor of the material, and you should see the pyramid in the color of the vertex
   colors.
   Change it back to `false`.

7. Divide the pyramid into five primitives corresponding to faces and base.
   Assign different colors to each of the primitives. Keep the same colors as before. It is best to add primitives one
   by one. 

   