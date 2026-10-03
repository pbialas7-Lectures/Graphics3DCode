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
    class Application {
    public:
        Application(int width, int height, std::string title, bool debug, int swap_interval=1);

        void run(int verbose = 0);

        void run_cli(int argc, char **argv);

        auto frame_buffer_size() const {
            int w, h;
            glfwGetFramebufferSize(window_, &w, &h);
            return std::make_pair(w, h);
        }

        void save_frame_buffer();

        virtual void init() {};

        virtual void init_cli(int argc, char **argv) {}

        virtual void frame() {}

        virtual void imgui() {}

        virtual void imgui_info() {}

        virtual void cleanup() {
            RegisteredObject::cleanup();
        }

        virtual void framebuffer_resize_callback(int w, int h) {}

        virtual void scroll_callback(double xoffset, double yoffset) {}

        virtual void cursor_position_callback(double x, double y) {}

        virtual void mouse_button_callback(int button, int action, int mods) {}

        virtual void key_callback(int key, int scancode, int action, int mods);

        virtual void window_refresh_callback() {};


    protected:
        GLFWwindow *window_;

    private:

        void startup(int verbose); // GL info, version check and debug output setup

        void shutdown();

        void loop(); // main loop

        unsigned int screenshot_n_;

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

        static void glfw_framebuffer_size_callback(GLFWwindow *window_ptr, int w, int h);

        static void glfw_scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

        static void glfw_cursor_position_callback(GLFWwindow *window, double x, double y);

        static void glfw_mouse_button_callback(GLFWwindow *window, int button, int action, int mods);

        static void glfw_key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);

        static void glfw_window_refresh_callback(GLFWwindow *window);
    };
}