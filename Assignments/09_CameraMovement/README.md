# Camera Movement

In the previous exercise we added the ability to "zoom" i.e., change the camera viewing angle using the mouse wheel. In
this exercise, we will add the ability to rotate the camera around the central point:

1. The central point will be the point given as the `center` parameter in the `look_at` function.
2. Horizontal mouse movement will rotate the camera around an axis parallel to the camera's y-axis (up direction) and
   passing through the central point.
3. Vertical mouse movement will rotate the camera around an axis parallel to the camera's x-axis (right direction) and
   passing through the central point.

This is a long exercise, but the ability to rotate the view freely around our model gives strange satisfaction :) I
suggest working in small steps, checking each time if the code still works. And don't go any further until everything is
OK.

## Camera

1. We will start by introducing changes in the `Camera` class. Instead of storing the view matrix `V_`, we will store
   the camera position `position_`, the central point `center_` and three vectors defining the camera
   orientation `x_`, `y_` and `z_`.

   Please add these fields to the `Camera` class.

2. Then please add to the  `look_at` method of the `Camera` class the code setting these variables as
   it was described in the lecture:

   ```c++
   z_ = glm::normalize(eye - center);
   x_ = glm::normalize(glm::cross(up, z_));
   y_ = glm::normalize(glm::cross(z_, x_));

   position_ = eye;
   center_ = center;
   ```

   We still need methods to read these values:

      ```c++
      glm::vec3 x() const { return x_; }
      glm::vec3 y() const { return y_; }
      glm::vec3 z() const { return z_; }
      glm::vec3 position() const { return position_; }
      glm::vec3 center() const { return center_; }
      ```

   Our code should still be compilable and work correctly. The next step is to change the code in the `view` method of
   the same class. Instead of returning the `V_` matrix, we will generate it from new variables "on demand," again as
   described in the lecture :)

   ```c++
   glm::mat4 view() const {
       glm::mat4 V(1.0f);
       for (int i = 0; i < 3; ++i) {
           V[i][0] = x_[i];
           V[i][1] = y_[i];
           V[i][2] = z_[i];
       }
       
       auto t = -glm::vec3{
               glm::dot(x_, position_),
               glm::dot(y_, position_),
               glm::dot(z_, position_),
       };
       V[3] = glm::vec4(t, 1.0f);
       
       return V;
       
       }
   ```  

If everything has been done correctly, the program should continue to work without changes. We can now remove the `V_`
field from the class.

## Rotations

Now we will implement methods for rotating the camera. Rotating the camera around an axis at the central point consists
of two steps:

1. Rotation of the camera position around this axis and point.
2. Rotation of all three vectors defining the camera orientation around this axis.


1. We will start by defining a helper function that creates a matrix describing a rotation by an angle around an
   axis passing through the origin.

   ```c++
   glm::mat3 rotation(float angle, const glm::vec3 &axis) {
       auto u = glm::normalize(axis);
       auto s = std::sin(angle);
       auto c = std::cos(angle);
   
       return glm::mat3(
           c + u.x * u.x * (1.0f - c),
           u.y * u.x * (1.0f - c) + u.z * s,
           u.z * u.x * (1.0f - c) - u.y * s,
   
           u.x *u.y*(1.0f-c)-u.z *s,
           c + u.y*u.y *(1.0f-c),
           u.z*u.y*(1.0f-c)+u.x*s,
   
           u.x*u.z*(1.0f -c)+ u.y*s,
           u.y*u.z*(1.0f-c)-u.x*s,
           c+u.z*u.z*(1.0f -c)
       );
   }     
   ```
   Please add this function to the anonymous namespace in the `camera.h` file.

2. The Method below rotates the camera by and
   angle around an axis passing through specified point `c`. The rotation matrix is applied to all
   three vectors defining the camera orientation. The position however has to be rotated by an axis passing through
   the specified point `c`, not the origin. To achieve this, we translate the position by `-c`, moving point `c` to the
   origin, then we
   rotate the translated position around the origin and translate it back by `c`, moving point `c` back to its original
   position.

   ```c++
   void rotate_around_point(float angle, const glm::vec3 &axis, const glm::vec3 &c) {
 
    auto R = rotation(angle, axis);
    x_ = R * x_;
    y_ = R * y_;
    z_ = R * z_;
    
    auto t = position_ - c;
    t = R * t;
    position_ = c + t;
    
   }
   ```

   Please add another auxiliary method that rotates the camera around the center:

   ```c++
   void rotate_around_center(float angle, const glm::vec3 &axis) {
        rotate_around_point(angle, axis, center_);
   }
   ```

   Those methods should be added inside the `Camera` class in the `camera.h` file.

## Controller

`Camera` class is ready, now we need to add mouse support. To do this, we will create a new `CameraController` class. In
this way, we will isolate the way the camera moves from the camera itself, and if the need arises, we will be able to
replace the controller with another one, e.g. FPC (First-Person Camera).

1. In the new header file `camera_controller.h` we create a class

   ```c++
   #include "camera.h"   

   class CameraController {
   public:
       CameraControler():camera_(nullptr) {}
       CameraControler(Camera* camera):camera_(camera) {}
       void set_camera(Camera *camera) { camera_ = camera; }
   
   private:
       Camera *camera_; 
   }
   ```

   Assuming that we have already obtained the mouse position change `dx` and `dy`, we rotate the camera using the
   method:

   ```c++
   void rotate_camera(float dx, float dy) {
       camera_->rotate_around_center(-scale_ * dy, camera_->x());
       camera_->rotate_around_center(-scale_ * dx, camera_->y());
       }
   ```

   `scale_` is another field of the `CameraControler` class and is a converter between pixels and angle. Its value
   should be selected experimentally for a given screen resolution.

   We slowly approach the end. Now we will implement three methods responsible for handling mouse events. First of all,
   we want the rotation to work only when we move the mouse holding down the left mouse button (LMB). Therefore, we will
   add another `LMB_pressed_` field of type `bool` to the class. In the class constructor, we should set its value
   to `false`. Because we will be tracking changes in the mouse position, we need two additional fields `x_` and `y_` of
   type `float`which will store the last mouse position. Having these fields, the mouse movement handling will be done
   by the method:

   ```c++
   void mouse_moved(float x, float y) {
       if (LMB_pressed_) {
           auto dx = x - x_;
           auto dy = y - y_;
           x_ = x;
           y_ = y;
   
           rotate_camera(dx, dy);
       }
   };
   ```   

   Two more methods will handle the mouse button press and release events:

   ```c++
   void LMB_pressed(float x, float y) {
       LMB_pressed_ = true;
       x_ = x;
       y_ = y;
   };
   
   void LMB_released(float x, float y) {
       LMB_pressed_ = false;
       auto dx = x - x_;
       auto dy = y - y_;
   
       rotate_camera(dx, dy);
   };
   ``` 

## Handling mouse events

Finally, we need to connect the controller methods to the event handling.
To do this:

1. Add a field

   ```c++
   CameraController *controller_;
   ```

   and a method

   ```c++
   void set_controler(CameraController *controller) { controller_ = controller; }
   ```

   to the `SimpleShapeApplication` class. Then in the `init` method we set the controller:

   ```c++
   set_controler(new CameraController(camera()));
   ```

2. And finally, we add event handling by overriding the methods:

   ```c++
   void mouse_button_callback(int button, int action, int mods) {
     Application::mouse_button_callback(button, action, mods);
   
     if (controller_) {
     double x, y;
     glfwGetCursorPos(window_, &x, &y);
   
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        controller_->LMB_pressed(x, y);
   
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
        controller_->LMB_released(x, y);
    }
   } 
   ```      

   ```c++
   void cursor_position_callback(double x, double y) {
     Application::cursor_position_callback(x, y);
     if (controller_) {
        controller_->mouse_moved(x, y);
     }
   }
   ```

Good luck :) 
