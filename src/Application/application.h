//
// Created by pbialas on 05.08.2020.
//
#pragma once

#include <iostream>
#include <iomanip>

#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>
#include "RegisteredObject.h"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

namespace xe {
    /**
     * @brief Base class of all the applications: it creates the window with an OpenGL context and runs the main loop.
     *
     * To write an application derive from this class and override the virtual methods you need, at least
     * init() and frame(). The methods are called in the following order:
     *
     *  1. the constructor creates the window, the OpenGL context and the ImGui context,
     *  2. run() (or run_cli(), which first calls init_cli()) calls init() once,
     *  3. then, until the window is closed, for each frame: frame(), imgui_info(), imgui(),
     *     followed by the swap of the buffers and the processing of the input events
     *     (which calls the *_callback methods),
     *  4. after the window is closed: cleanup(),
     *  5. the destructor destroys the window and the OpenGL context.
     *
     * Built-in keyboard shortcuts, not passed to key_callback():
     *  - Ctrl-Q closes the window,
     *  - Ctrl-S saves the next frame to screenshot_<n>.png (see save_frame_buffer()),
     *  - Ctrl-F captures the next frame in RenderDoc, if the application was started from RenderDoc (Linux only).
     *
     * Mouse and keyboard presses are not passed to the callbacks while ImGui uses them, e.g. when the mouse is over
     * an ImGui window or a text field has the keyboard focus. Releases are always passed.
     */
    class Application {
    public:
        /**
         * @brief Creates the window, the OpenGL context and the ImGui context. Exits the program on failure.
         *
         * @param width Width of the window.
         * @param height Height of the window.
         * @param title Title of the window.
         * @param debug If true, an OpenGL debug context is created and its debug messages are logged.
         * @param swap_interval Number of screen refreshes to wait for before swapping the buffers:
         *                      1 synchronizes the frame rate with the screen (v-sync), 0 renders as fast as possible.
         */
        Application(int width, int height, std::string title, bool debug, int swap_interval=1);

        /**
         * @brief Destroys the window and the OpenGL context.
         *
         * This runs after the members of a derived application have been destroyed, so members owning OpenGL
         * objects are deleted while the context still exists.
         */
        virtual ~Application();

        // The window keeps a pointer to the application, so the application can be neither copied nor assigned.
        Application(const Application &) = delete;

        Application &operator=(const Application &) = delete;

        /**
         * @brief Runs the application: calls init(), then runs the main loop until the window is closed,
         * then calls cleanup().
         *
         * @param verbose If greater than zero, the OpenGL vendor, renderer and version are logged.
         */
        void run(int verbose = 0);

        /**
         * @brief Like run(), but first parses the command line.
         *
         * The `-v`/`--verbose [level]` option sets the verbosity as in run(). The program name and all the
         * arguments not recognized here are passed to init_cli() before init() is called.
         */
        void run_cli(int argc, char **argv);

        /**
         * @brief Returns the size of the framebuffer as a (width, height) pair.
         *
         * The size is in pixels and is the one to use with glViewport. It may differ from the window size
         * given in the constructor, e.g. on high resolution screens.
         */
        auto frame_buffer_size() const {
            int w, h;
            glfwGetFramebufferSize(window_, &w, &h);
            return std::make_pair(w, h);
        }

        /**
         * @brief Saves the content of the back buffer to screenshot_<n>.png, where n counts the screenshots.
         *
         * Call it after rendering the frame and before the buffers are swapped. Pressing Ctrl-S does this
         * automatically after the next frame(), so the screenshot does not contain the ImGui windows.
         */
        void save_frame_buffer();

        /**
         * @brief Called once before the main loop starts. Override it to create the OpenGL objects:
         * buffers, vertex arrays, shader programs, textures, ...
         */
        virtual void init() {};

        /**
         * @brief Called by run_cli() before init() with the command line arguments not used by run_cli().
         * argv[0] is the program name.
         */
        virtual void init_cli(int argc, char **argv) {}

        /**
         * @brief Called in every iteration of the main loop to render the frame. Override it with your
         * rendering code.
         *
         * The color and depth buffers are already cleared when this is called.
         */
        virtual void frame() {}

        /**
         * @brief Called in every frame after frame(). Override it to create your own ImGui windows.
         */
        virtual void imgui() {}

        /**
         * @brief Called in every frame while the "Info" window in the top left corner, showing the frame rate,
         * is being drawn. Override it to add your own lines to this window, e.g. with ImGui::Text.
         */
        virtual void imgui_info() {}

        /**
         * @brief Called once after the main loop ends, while the OpenGL context still exists.
         *
         * The default implementation deletes all the RegisteredObject instances (meshes, materials, ...).
         * If you override it, call Application::cleanup() at the end.
         */
        virtual void cleanup() {
            RegisteredObject::cleanup();
        }

        /**
         * @brief Called when the framebuffer is resized.
         *
         * @param w New width of the framebuffer in pixels.
         * @param h New height of the framebuffer in pixels.
         */
        virtual void framebuffer_resize_callback(int w, int h) {}

        /**
         * @brief Called when the mouse wheel or the touchpad is scrolled.
         *
         * @param xoffset Horizontal scroll offset.
         * @param yoffset Vertical scroll offset, the one changed by an ordinary mouse wheel.
         */
        virtual void scroll_callback(double xoffset, double yoffset) {}

        /**
         * @brief Called when the mouse cursor moves.
         *
         * @param x Cursor position relative to the left edge of the window content area.
         * @param y Cursor position relative to the top edge of the window content area (y grows downwards).
         */
        virtual void cursor_position_callback(double x, double y) {}

        /**
         * @brief Called when a mouse button is pressed or released.
         *
         * @param button GLFW_MOUSE_BUTTON_LEFT, GLFW_MOUSE_BUTTON_RIGHT, GLFW_MOUSE_BUTTON_MIDDLE, ...
         * @param action GLFW_PRESS or GLFW_RELEASE.
         * @param mods Bit field of the modifier keys held down, e.g. GLFW_MOD_SHIFT, GLFW_MOD_CONTROL.
         */
        virtual void mouse_button_callback(int button, int action, int mods) {}

        /**
         * @brief Called when a key is pressed, repeated or released. The built-in Ctrl shortcuts
         * (see the class description) are not passed here.
         *
         * @param key GLFW_KEY_A, GLFW_KEY_SPACE, ...; independent of the keyboard layout.
         * @param scancode Platform specific code of the key.
         * @param action GLFW_PRESS, GLFW_REPEAT or GLFW_RELEASE.
         * @param mods Bit field of the modifier keys held down, e.g. GLFW_MOD_SHIFT, GLFW_MOD_CONTROL.
         */
        virtual void key_callback(int key, int scancode, int action, int mods) {}

        /**
         * @brief Called when the content of the window has to be redrawn, e.g. while the window is being
         * resized on some platforms, when the main loop is not running.
         */
        virtual void window_refresh_callback() {};


    protected:
        // The GLFW window, for calling GLFW functions directly, e.g. glfwSetWindowShouldClose(window_, 1).
        GLFWwindow *window_;

    private:

        void startup(int verbose); // GL info, version check and debug output setup

        void shutdown(); // ImGui shutdown and cleanup(); the context itself is destroyed in the destructor

        bool shut_down_ = false;

        void loop(); // main loop

        unsigned int screenshot_n_; // number of the next screenshot
        bool screenshot_requested_ = false; // set by Ctrl-S, the screenshot is taken after the next frame()

        // RenderDoc in-application capture support (see application.cpp).
        // The API pointer is stored as void* here so that renderdoc_app.h does not
        // have to be included in this widely-used header; it is cast back to the
        // proper RENDERDOC_API_1_x_x* type in application.cpp.
        void init_renderdoc();

        void renderdoc_start_capture();

        void renderdoc_end_capture();

        void *renderdoc_api_ = nullptr;
        bool renderdoc_capture_requested_ = false;
        bool renderdoc_capturing_ = false;

        // GLFW calls these static functions; they find the Application object through the window user pointer
        // and call the corresponding virtual *_callback methods.

        static void glfw_framebuffer_size_callback(GLFWwindow *window_ptr, int w, int h);

        static void glfw_scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

        static void glfw_cursor_position_callback(GLFWwindow *window, double x, double y);

        static void glfw_mouse_button_callback(GLFWwindow *window, int button, int action, int mods);

        static void glfw_key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);

        static void glfw_window_refresh_callback(GLFWwindow *window);
    };
}
