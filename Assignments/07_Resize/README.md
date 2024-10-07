# Resize

In this exercise we will change behavior of the application when resizing the window. But first let's gain some more 3D
perspective on the pyramid.
Please change the location of the camera in the `init` method of the `SimpleShapeApplication` class to (2.0,1.0,2.0)
looking at (0,0,0) with up vector (0,0,1).
Now the pyramid should be visible from the side.

Try to resize the display window. Currently, resizing the window changes the canvas size of the displayed image. The
pyramid remains all the time in the same place without changing. If we reduce the size of the window some portions of
the pyramid may be cut out. This happens because the size of the viewport was set once in the `init` method and is not
changed during the application execution. The portions of the display window (framebuffer) that are outside the viewport
are just filled with background color. The portions of the vieport that are outside the framebuffer are simply not
displayed. We will change this behavior as to rescale the displayed objects when the window is resized.

For this purpose, we will use the virtual void method I prepared: `Application::framebuffer_resize_callback (int w, int
h)`, which is called during each frame buffer size change with the `w` and `h` parameters defining the new buffer sizes.
This method can be overridden in classes derived from the `Application` class, such as `SimpleShapeApp`, which you use
in
your exercises.

1. We will start with resizing the viewport to the size of the framebuffer each time framebuffer is resized. To do this
   first add
   ```c++
   void framebuffer_resize_callback(int w, int h) override;
   ```
   to the `public` part of the `SimpleShapeApplication` class declaration in the `app.h` file indicating that we will
   substitute our own implementation of this method.
   Then add the definition in the  `app.cpp` file:
   ```c++
   void SimpleShapeApplication::framebuffer_resize_callback(int w, int h) {
       Application::framebuffer_resize_callback(w, h);
   }
   ```
   This will not yet change the behavior of the application, so add the `glViewport` call to this method to set the
   viewport to the size of the framebuffer.

   Try to resize the window. What happens? If you have done everything correctly, the pyramid should scale with the size
   of the window. However, the proportions of the pyramid are not preserved. That is because the aspect ratio of the
   viewport no longer matches the aspect ratio of the perspective projection set up in the call the `glm::perspective`
   function in the `init` method.

2. To preserve the proportions of the pyramid, we need to change the aspect ratio of the perspective projection each
   time
   the framebuffer is resized. Because that means changing the perspective matrix from within
   the `framebuffer_resize_callback` method, this matrix must be accessible there, so it cannot be defined in the `init`
   method anymore. Another issue is that once we change the perspective matrix, we need to send the new PVM matrix to
   the shader.

   To solve this problem, we will define several new fields in the `SimpleShapeApplication` class in the `app.h` file
   (you will need to include the `glm/glm.hpp` header file):

   ```c++
   float fov_;
   float aspect_;
   float near_;
   float far_;
   
   glm::mat4 P_;
   glm::mat4 V_;
   glm::mat4 M_;
   
   ```

   In the `init` method of the `app.cpp` please assign same values to them as before and change code accordingly to use
   those new fields.  
   Please delete the unnecessary code from the `init` method i.e., the `M,` `V` and `P` variables.

   At this point we still create the PVM matrix in the  `init` method and send it to the uniform buffer here.
   We will change this in the next step.

3. The PVM matrix needs to be recalculated and loaded to uniform buffer every time the framebuffer is resized. We could
   do this in the `framebuffer_resize_callback` method, but in anticipation of further use, we will move this code to
   the `frame` method. For that to be succesful we need a way to access the uniform buffer handle in this method, so
   again we will create a new field in the `SimpleShapeApplication` class in the `app.h` file:
   ```c++
   GLuint u_trans_buffer_handle_;
   ```  
   Now in the `init` method use this variable to store the handle of the uniform buffer. Change the code accordingly,
   and remove the old handle variable from the `init` method. Check if everything works as before.

   So please move the `P_` and `PVM` calculation to the `frame` method and remove the old code from the `init` method
   and load the matrix to the transformations uniform buffer using the `glNamedBufferSubData`

5. Then bind the uniform buffer using the `glBindBufferBase` method and
   method. Remove this code from the `init` method. After executing the draw call unbind the buffer using
   the `glBindBufferBase` method.

4. Finally, in the `framebuffer_resize_callback` add the code that changes the `aspect_`. This will be used to calculate
   the new perspective matrix in the `frame` method. The `fov_`, `near_` and `far_` values should remain unchanged.

   Now try to resize the window.
   The pyramid should scale with the window and its proportions should be preserved.
   To be more precise, the pyramid will scale only with the horizontal size of the window.
   The vertical size will be scaled accordingly to preserve the aspect ratio. 
   
  
  