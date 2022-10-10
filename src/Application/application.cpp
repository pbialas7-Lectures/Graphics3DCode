/**
 * @file application.cpp
 * @author Piotr Białas (piotr.bialas@uj.edu.pl)
 * @brief 
 * @version 0.1
 * @date 2021-10-01
 * 
 * @copyright Copyright (c) 2021
 * 
 */

//
// Created by pbialas on 16.08.2020.
//

#include "Application/application.h"

#include <iostream>
#include <sstream>
#include <tuple>

#include "glad/gl.h"
#include "utils.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "stb/stb_image_write.h"

/**
 * @brief Predefined debuging callbacks.
 * 
 * If generated with debug option GLAD  permits to register callbacks that will be called before and after each OpenGL function call. 
 * This is switched off by default by me, so to not interfere with my error reporting code.  GLAD debuging can be enabled in the CMakeLists.txt file.
 * 
 * This unnamed namespace contains two predefined postcall callbacks making them local to this file.  
 * 
 */
namespace
{
    /**
     * @brief 
     * 
     * @param ret 
     * @param name 
     * @param apiproc 
     * @param len_args 
     * @param ... 
     */

    void _pre_call_callback(const char *name, GLADapiproc apiproc, int len_args, ...) {
        //std::cerr<<"Calling "<<name<<"\n";
    };
    void _post_call_callback_default(void *ret, const char *name, GLADapiproc apiproc, int len_args, ...)
    {
        GLenum error_code;
        error_code = glad_glGetError();

        if (error_code != GL_NO_ERROR)
        {
            std::cerr << "ERROR " << error_code << " " << xe::utils::error_msg(error_code) << " in " << name << std::endl;
        }
    }
    void _post_call_callback_no_debug(void *ret, const char *name, GLADapiproc apiproc, int len_args, ...)
    {
    }

}

/**
 * @brief Construct a new xe::Application::Application object
 * 
 * @param width  Width of the window    
 * @param height Height of the window
 * @param title Title of the created application window. 
 * @param debug specify if the debug information should be generated after each OpenGL function call
 *              has efect only if compiled with debug version of glad.     
 */
xe::Application::Application(int width, int height, std::string title, bool debug) : screenshot_n_(0)
{

    if (glfwInit())
    {

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, MAJOR);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, MINOR);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
        glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);

        window_ = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
        if (!window_)
        {
            const char *error_desc;
            auto err_code = glfwGetError(&error_desc);
            std::cerr << "Cannot create window: " << error_desc << "\n";
            glfwTerminate();
            exit(-1);
        }
        glfwMakeContextCurrent(window_);
        glfwSetWindowUserPointer(window_, this);

        glfwSetFramebufferSizeCallback(window_, Application::glfw_framebuffer_size_callback);
        glfwSetScrollCallback(window_, Application::glfw_scroll_callback);
        glfwSetCursorPosCallback(window_, Application::glfw_cursor_position_callback);
        glfwSetMouseButtonCallback(window_, Application::glfw_mouse_button_callback);
        glfwSetKeyCallback(window_, Application::glfw_key_callback);
        glfwSetWindowRefreshCallback(window_, glfw_window_refresh_callback);

#ifdef GLAD_OPTION_GL_DEBUG
        std::cerr << "GLAD_OPTION_GL_DEBUG\n";
        //Additionally if GLAD debugging is on, the we can still switch it off via debug variable.
        //This works by registering an empty predefined above callback.  
        if (debug) {
            std::cerr << "DEBUG ON\n";
            gladSetGLPreCallback(_pre_call_callback);
            gladSetGLPostCallback(_post_call_callback_default);
        }
        else
        {
            std::cerr << "NO DEBUG\n";
            gladSetGLPostCallback(_post_call_callback_no_debug);
        }
#endif

        if (!gladLoadGL(glfwGetProcAddress))
        {
            std::cout << "Failed to initialize OpenGL context " << MAJOR << "." << MINOR << std::endl;
            exit(-1);
        }

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glfwSwapInterval(1);
    }
}

/**
 * @brief This starts the main event loop. 
 * 
 * @param verbose unused parameter. 
 */
void xe::Application::run(int verbose)
{

    if (verbose > 0)
    {
        std::cout << utils::get_gl_description() << "\n";
    }
    init();

    auto macMoved = false;
    while (!glfwWindowShouldClose(window_))
    {

        //Clear the framebuufer by filling it with color set using the glClearColor function. 
        //Also clears the depth buffer. 
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //This method should be overidden by you and will contain the rendering code.
        frame();
        /* Swap front and back buffers 
           The rendering is done into the BACK buffer, swapping it with front buffer displays it on the screen.  
           This is done after n screen updates where n is the number set by the glfwSwapInterwal. 
           Setting it to one as I did set the swap rate to v-sync rate. 
        */
        glfwSwapBuffers(window_);

        /* Poll for and process events */
        glfwPollEvents();
#ifdef __APPLE__
        // A hack to fix bug in apple implementation. 
        // maybe not needed now. Didn't check :( 
        if (!macMoved)
        {
            int x, y;
            glfwGetWindowPos(window_, &x, &y);
            glfwSetWindowPos(window_, ++x, y);
            glfwSetWindowPos(window_, --x, y);
            macMoved = true;
        }
#endif
    }

    cleanup();
    glfwTerminate();
}

void xe::Application::glfw_framebuffer_size_callback(GLFWwindow *window_ptr, int w, int h)
{
    auto app_ptr = reinterpret_cast<Application *>(glfwGetWindowUserPointer(window_ptr));
    if (app_ptr)
    {
        app_ptr->framebuffer_resize_callback(w, h);
    }
}

void xe::Application::glfw_scroll_callback(GLFWwindow *window_ptr, double xoffset, double yoffset)
{
    auto app_ptr = reinterpret_cast<Application *>(glfwGetWindowUserPointer(window_ptr));
    if (app_ptr)
    {
        app_ptr->scroll_callback(xoffset, yoffset);
    }
}

void xe::Application::glfw_cursor_position_callback(GLFWwindow *window, double x, double y)
{
    auto app_ptr = reinterpret_cast<Application *>(glfwGetWindowUserPointer(window));
    if (app_ptr)
    {
        app_ptr->cursor_position_callback(x, y);
    }
}

void xe::Application::glfw_mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{

    auto app_ptr = reinterpret_cast<Application *>(glfwGetWindowUserPointer(window));
    if (app_ptr)
    {
        app_ptr->mouse_button_callback(button, action, mods);
    }
}

void xe::Application::glfw_key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_Q && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, 1);
    }
    auto app_ptr = reinterpret_cast<Application *>(glfwGetWindowUserPointer(window));
    if (app_ptr)
    {
        app_ptr->key_callback(key, scancode, action, mods);
    }
}

void xe::Application::key_callback(int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_S && action == GLFW_PRESS)
    {
        save_frame_buffer();
    }
}

void xe::Application::save_frame_buffer()
{
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glReadBuffer(GL_FRONT);
    if (glGetError() == GL_INVALID_OPERATION) 
        std::cerr << "Saving Frame buffer error: Front buffer does not exist."<<std::endl;

    auto[w,h] = frame_buffer_size();
    auto data = (GLubyte *)malloc(w * h * 3);
    OGL_CALL(glReadPixels(0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, data));
    
    stbi_flip_vertically_on_write(1);
    std::stringstream ss;
    ss << "screenshot_" << std::setw(3) << std::setfill('0') << screenshot_n_ << ".png";
    stbi_write_png(ss.str().c_str(), w, h, 3, data, w * 3);
    ++screenshot_n_;
    std::cerr<<ss.str()<<"\n"; 
}

void xe::Application::glfw_window_refresh_callback(GLFWwindow *window)
{

    auto app_ptr = reinterpret_cast<Application *>(glfwGetWindowUserPointer(window));
    if (app_ptr)
    {
        app_ptr->window_refresh_callback();
    }
}

void xe::Application::window_refresh_callback() {}
