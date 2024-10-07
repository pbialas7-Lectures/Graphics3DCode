<style>
.center {
  display: block;
  margin: 1em auto;
  width: 50%;
}
</style>

# Projection - View - Model

#### 5 points

The aim of this assignment is to familiarize you with model, view and projection transforms.
The model transformation (M) is responsible for placement of the model in the scene, the view transformation (V)is
responsible for camera placement and orientation, and the projection (P) is responsible for the field of view of the
camera.

All those three transformations are performed by 4x4 matrices multiplying the homogenous coordinates of the vertices.
Homogenous coordinates are four dimensional vectors

```
x y z w
```

and actual 3D coordinates are obtained as

```
x/w y/w z/w
```

For vertices before the transformation the coordinate `w` is equal to 1. Actually only the projection transformation
will change the `w`.

All those three transformations are rolled into one matrix

``` 
PVM = P*V*M
```

that we will put into a uniform interface block.

1. In vertex shader change the `Transformations` interface block to
   ```c++
   layout(std140, binding=1) uniform Transformations {
       mat4 PVM;
   };
   ```
   and remove the transformation code from `main` function, just leave
   ```glsl
   gl_Position = a_vertex_position;
   ```

2. In the `init` method create a unit `PVM` matrix
   ```c++
   glm::mat4 PVM(1.0f); 
   ```  
3. If needed, modify the size of the uniform buffer associated with this interface block, and load the `PVM` matrix into
   this buffer.
   The 4x4 matrix can be loaded in "one go" with a single call to `glNamedBufferSubData`. The pointer to the
   data in the matrix can be obtained as `&PVM[0]`.
   Remove the old unused code, do not leave it just commented.

4. In the `main` function of the vertex shader use the `PVM` matrix to transform vertex coordinates:
   ```glsl
   gl_Position =  PVM * a_vertex_position;
   ``` 

This should change nothing in the display. Please remove the old code.

1. Now compose the `PVM` matrix from Model, View and Projection matrices. Start with identity model
   matrix.
   ```c++
   glm::mat4 M(1.0);
   ```
   Include the `glm/gtc/matrix_transform.hpp`  header file. Now you can use the `glm::perspective` function
   to create the projection matrix and `glm::lookAt` function to create view matrix.
   Familiarize yourself with the parameters of those functions.

2. Using the `glm::lookAt` function set the camera position (V matrix) at (0,-2,2) looking at (0,0,0) with up vector (
   0,0,1).
   Next using the glm::perspective function set the projection matrix (P matrix) to have 45 degrees field of view,
   aspect ratio equal to the aspect ratio of the window, near plane at 0.1 and far plane at 20.0.
   You can use function `glm::radians` to convert degrees to radians. To obtain the width and height of the window
   needed for calculating the aspect ratio use the `frame_buffer_size` function. It is already used at the end of
   the `init` function. Just move it to where you need it. When calculating the aspect ratio you will need to cast the
   width and height to float. In other case the integer division will be performed and the result will be wrong.

3. Now multiply the matrices in the correct order to obtain the `PVM` matrix and load it into the uniform buffer.
   Finally, translate the house by (-1,1,0) using the model matrix. This should move the house to the left and up. Use
   the `glm::translate` function to create this matrix.

   The final result should look like this:
   <img alt="House" src="house.png" class="center">