//
// Created by pbialas on 05.08.2020.
//
#pragma once

#include <iostream>
#include <iomanip>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace xe
{
    class Application
    {
    public:
        Application(int width, int height, std::string title, bool debug );

        void run(int verbose = 0);

        auto frame_buffer_size() const
        {
            int w, h;
            glfwGetFramebufferSize(window_, &w, &h);
            return std::make_pair(w, h);
        }

        void save_frame_buffer();

        virtual void init(){};

        virtual void frame() {}

        virtual void cleanup() {}

        virtual void framebuffer_resize_callback(int w, int h) {}

        virtual void scroll_callback(double xoffset, double yoffset) {}

        virtual void cursor_position_callback(double x, double y) {}

        virtual void mouse_button_callback(int button, int action, int mods) {}

        virtual void key_callback(int key, int scancode, int action, int mods);

        virtual void window_refresh_callback();

       

    protected:
        GLFWwindow *window_;

    private:
        unsigned int screenshot_n_;

        static void glfw_framebuffer_size_callback(GLFWwindow *window_ptr, int w, int h);

        static void glfw_scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

        static void glfw_cursor_position_callback(GLFWwindow *window, double x, double y);

        static void glfw_mouse_button_callback(GLFWwindow *window, int button, int action, int mods);

        static void glfw_key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);

        static void glfw_window_refresh_callback(GLFWwindow *window);
    };
}