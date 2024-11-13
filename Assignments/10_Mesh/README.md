# Mesh

So far, we did everything on the most basic level of OpenGL calls, but this would quickly become unwieldy if we would
like to add more models.
In this assignment, you will refactor the code by adding some layers of abstraction.
We will add a `Mesh` class that will encapsulate all this low-level calls concerned with vertex and index buffer
manipulations.
This class is already provided for you in the `src/Engine/` directory.

But we will start with another refactorisation that will consist of moving the `Camera` and `CameraController` classes
to the `src/Engine` directory, so
they can be accessible to all further assignments without a need to copy them:

1. So go ahead and copy the `09_CameraMovement` assignment
   to `10_Mesh`.

2. Then move the files `camera.h` and `camera_controller.h`  from `src/Assignament/10_Mesh` to the
   directory `src/Engine`.


4. Put the `Camera` and `CameraController` classes into the `xe` namespace, by surrounding their code by the namespace
   declaration:

   ```c++
   namespace xe {

   }
   ```
   Modify all the references to those classes and its methods by prefixing them with `xe::`.

3. Now change the `#include` directives that include those files so to reflect the new path.
   If you use `CLion` or another IDE this may have been done for you already.

5. Finally, in the `CMakeLists.txt` file in `src/10_Mesh` directory modify  `target_link_libraries` function to link
   the `Engine` library

    ```cmake
    target_link_libraries(${PROJECT_NAME} PUBLIC Engine spdlog::spdlog)
    ```
   Everything should still work as before.

## Mesh class

`Mesh` class corresponds to a single `Vertex Array Object` (VAO) containing one vertex buffer and one index buffer.
Every object of this class must contain at least one object of `Mesh::SubMesh` class.
`SubMesh` class corresponds to a single call to `glDrawElements` function and contains the range of the indices to be
submitted to this call. `SubMesh` also contains a pointer to the `Material` that will be used in next assignments to
specify the kind of material used by this primitive which in turn will determine the color of the pixels. By default,
this pointer is initialized to `null_material` which is a pointer to `NullMaterial`
that does nothing.
Submeshes are added to mesh using the `add_submesh` method.

Mesh class also assumes using a predefined set of vertex attributes that are specified by the `xe::Attribute` enum.
The values of the enum correspond to the location value of the corresponding attribute in vertex shader.
Adding the attribute to the mesh is done using the `add_attribute` method.
This method is similar to the `glVertexAttribPointer` function, but it does not require the `stride` argument as it is
already provided in the constructor (see description below), and uses the `Attribute` enum instead of the attribute index.

1. We will store the meshes in the vector of meshes in the `SimpleShapeApplication` class, so please add the field
   ```c++
   std::vector<Mesh*> meshes_; 
   ```
   to this class in `app.h` file and a corresponding `add_mesh` method:

   ```c++
   void add_mesh(Mesh *mesh) {
       meshes_.push_back(mesh);
   }
   ```
2. In the vertex shader change the location of the `a_vertex_color` to five (5). Change corresponding parameter in call
   to `glVertexAttribPointer`.

3. Next we have to create the mesh for our pyramid in the `init` method. So start with creating a new `Mesh`
   ```c++
   auto pyramid = new xe::Mesh(6 * sizeof(float), vertices.size() * sizeof(float), GL_STATIC_DRAW,
                                indices.size() * sizeof(GLubyte), GL_UNSIGNED_BYTE, GL_STATIC_DRAW);
   ```
   The constructor of the `Mesh` takes as arguments:
    1. The stride in the vertex buffer, i.e., the size of the single vertex data(attributes) in bytes.
    2. The size of the vertex buffer in bytes.
    3. The usage hint of the vertex buffer.
    4. The size of the index buffer in bytes.
    5. The type of the indexes stored in the index buffer.
    6. The usage hint of the index buffer.

   Those values cannot be changed after the creation of the mesh.

4. Load the vertices using the `load_vertices_method` and set the layouts for attributes using `add_attribute`
   method.

5. Similarly, load data into the index buffer using `load_indices` method.

6. Add a submesh to this mesh that will contain all the indices and schedule mesh for drawing using
   the `SimpleShapeApplication::add_mesh` method.

7. Finally, in the method frame change the call to `glDrawElements`to a loop that will call `draw` method on each mesh
   in`meshes_`

   ```c++
    for (auto m: meshes_)
        m->draw();
   ```
   Remove the bind and unbind `vao_` calls from this method.

8. Please delete all unnecessary code from the  `init` method.    
