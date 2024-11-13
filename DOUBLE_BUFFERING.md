# Double buffering

The GLFW library creates a double buffered window by default.
This means that the window has two buffers, one that is displayed and one that is drawn to.
When the drawing is finished, the buffers are swapped.
This is done to prevent flickering/tearing.
The swapping is done with the function

```C++
glfwSwapBuffers(window)`.
```

It is possible to set the minimum number of monitor refreshes the driver should wait before swapping the buffers.
This can be set with the function

```C++
glfwSwapInterval(interval)`.
``` 

The default value of `interval` is 1, which means that the driver waits for the vertical retracing before swapping the
buffers.
Setting it to zero meas switching the buffers immediately i.r. turning of vsync.
However, on some platforms this may not have any effect as the driver may force vsync.

The value of the `interval` is provides as the last argument of the `Application` constructor and defaults to 1.
In your assignments you can change this in the `app.h` files in the `SimpleShapeApplication` constructor.

On Linux systems, even with double buffering enabled, there is a visible tearing effect at least on NVIDIA cards.
To alleviate this you have to switch on the full composition pipeline option in the NVIDIA settings.
Other cards may have similar settings.