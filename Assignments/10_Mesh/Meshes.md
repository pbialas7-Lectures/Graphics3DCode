# Mesh

In this assignment, we will do some code refactoring.

1. First we will move the `Camera` and `CameraController` files into the `src/Engine` directory, so they can be
   accessible
   to all further assignments without a need to copy them. So go ahead and copy the `09_CameraMovement` assignment
   to `10_Mesh`.
2. Then move the files `camera.h` and `camera_controller.h` to the directory `src/Engine`. Please add them to
   the `CMakeLists.txt` file in this directory in the `add_library` function.

3. Now change the `#include` directives that
   include those files so to reflect the new path. If you use `CLion` or another IDE this may have been done for you
   already.

4. Put the `Camera` and `CameraController` classes into the `xe` namespace, by surrounding their code by the namespace
   declaration:

   ```c++
   namespace xe {


   }
   ```
   Modify all the references to those classes and its methods by prefixing them with `xe::`.

5. Finally, in the `CMakeLists.txt` file in `src/10_Mesh` directory modify  `target_link_libraries` function to link
   the `Engine` library

    ```cmake
    target_link_libraries(${PROJECT_NAME} PUBLIC Engine spdlog::spdlog)
    ```
   Everything should still work as before.

## Mesh class

So far we did everything on the most basic level, this would quickly become unwieldy if we would like to add more
models. So we will add a `Mesh` class that will encapsulate all this low-level model manipulations. This class is
provided for you in the `src/Engine/` directory.

`Mesh` class corresponds to a single `Vertex Array Object` (VAO) containing one vertex buffer and one index buffer.
Every object of this class must contain at least one object of `SubMesh` class.
`Submesh` class corresponds to a single call to `glDrawElements` function and contains the range of the indices to be
submitted to this call. Submeshes are added to mesh using the `add_submesh` method.

1. We will store the meshes in the vector of Meshes in the `SimpleShapeApplication` class, so please add the field
   ```c++
   std::vector<Mesh*> meshes_; 
   ```
   to this class and a corresponding `add_mesh` method:

   ```c++
   void add_mesh(Mesh *mesh) {
       meshes_.push_back(mesh);
   }
   ```

2. Next we have to create the mesh for our pyramid in the `init` method. So start with creating a new `Mesh`
   ```c++
   auto pyramid = new xe::Mesh; 
   ```
3. Using the method `allocate_vertex_buffer` allocate memory for the vertex buffer and load the vertices using
   the `load_vertices_method` and set the layouts for attributes using `vertex_attrib_pointer` method.

4. Similarly, allocate memory and load data into the index buffer.

5. Add a submesh to this mesh that will contain all the indices and schedule mesh for drawing using the `add_mesh`
   method.

6. Finally, in the method frame change the call to `glDrawElements`to a loop that will call `draw` method on each mesh
   in`messhes_`

   ```c++
    for (auto m: meshes_)
        m->draw();
   ```
   Remove the bind and unbind `vao_` calls.    

7. Please delete all unnecessary code from the  `init` method.    
