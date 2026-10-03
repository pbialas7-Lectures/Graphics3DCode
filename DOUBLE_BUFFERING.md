# Double buffering

The GLFW library creates a double buffered window by default.
This means that the window has two buffers: the front buffer, which is displayed, and the back buffer, which is drawn
to. When the drawing is finished, the buffers are swapped.
This way you never see a partially drawn frame, which prevents flickering.
The swapping is done with the function

```C++
glfwSwapBuffers(window);
```

which the `Application` class calls at the end of each frame.

## Swap interval and v-sync

If the buffers are swapped while the monitor is in the middle of displaying a frame, the upper part of the screen shows
the old frame and the lower part the new one. When objects move, this is visible as a horizontal "tear" in the image.
To avoid it, the swap can wait for the vertical retrace, the moment the monitor starts displaying a new frame.
This is called vertical synchronization (v-sync).

The minimum number of monitor refreshes the driver should wait for before swapping the buffers is set with the function

```C++
glfwSwapInterval(interval);
```

An interval of 1 waits for the next vertical retrace, i.e. turns v-sync on and avoids tearing.
An interval of 0 swaps the buffers immediately, i.e. turns v-sync off; the program then renders as fast as it can,
which is useful for measuring performance.
With v-sync on, the frame rate shown in the "Info" panel is usually equal to the refresh rate of the monitor,
e.g. 60 FPS.

The `Application` class calls `glfwSwapInterval` with the value of `swap_interval`, the last argument of its
constructor, which defaults to 1. In your assignments it is passed in the `main.cpp` file:

```C++
SimpleShapeApplication app(650, 480, PROJECT_NAME, true, 1);
```

Change the last argument to 0 to turn v-sync off.
Note that some drivers have user settings that override the swap interval requested by the application, so changing
it may have no effect.

## Tearing on Linux with NVIDIA cards

On Linux with the X11 window system there can be visible tearing on NVIDIA cards even with v-sync on.
To alleviate this, switch on the "Force Full Composition Pipeline" option in the NVIDIA X Server Settings.
Other cards may have similar settings.
Under Wayland the compositor takes care of this.
