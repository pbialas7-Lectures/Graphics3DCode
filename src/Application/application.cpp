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

#include "spdlog/spdlog.h"
#include "glad/gl.h"
#include "cxxopts.hpp"

#include "utils.h"
#include "debug.h"

#include "stb/stb_image_write.h"

/**
 * @brief Predefined debugging callbacks.
 * 
 * If generated with debug option GLAD  permits to register callbacks that will be called before and after each OpenGL function call. 
 * This is switched off by default by me, as not to interfere with my error reporting code.  GLAD debuging can be enabled in the CMakeLists.txt file.
 * 
 * This unnamed namespace contains two predefined post-call callbacks making them local to this file.
 * 
 */
namespace {
    void _pre_call_callback(const char *name, GLADapiproc apiproc, int len_args, ...) {
    };

    void _post_call_callback_default(void *ret, const char *name, GLADapiproc apiproc, int len_args, ...) {
        GLenum error_code;
        error_code = glad_glGetError();

        if (error_code != GL_NO_ERROR) {
            spdlog::error("ERROR {} {} in {}", error_code, xe::utils::error_msg(error_code), name);
        }
    }

    void _post_call_callback_no_debug(void *ret, const char *name, GLADapiproc apiproc, int len_args, ...) {
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
xe::Application::Application(int width, int height, std::string title, bool debug, int swap_interval)
        : screenshot_n_(0) {
    SPDLOG_INFO("Application::Application({}, {}, {}, {}, {})", width, height, title, debug, swap_interval);
    if (glfwInit()) {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, MAJOR);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, MINOR);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
        glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

        window_ = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
        if (!window_) {
            const char *error_desc;
            auto err_code = glfwGetError(&error_desc);
            SPDLOG_CRITICAL("Cannot create window: {} {}", err_code, error_desc);
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
        SPDLOG_INFO("GLAD_OPTION_GL_DEBUG is ON");
        // Additionally if GLAD debugging is on, the we can still switch it off via debug variable.
        // This works by registering an empty predefined above callback.
        if (debug) {
            SPDLOG_INFO("DEBUG is ON, setting callbacks");
            gladSetGLPreCallback(_pre_call_callback);
            gladSetGLPostCallback(_post_call_callback_default);
        }
        else {
            SPDLOG_INFO("DEBUG is OFF");
            gladSetGLPostCallback(_post_call_callback_no_debug);
        }
#endif

        if (!gladLoadGL(glfwGetProcAddress)) {
            SPDLOG_CRITICAL("Failed to initialize OpenGL {}.{} context", MAJOR, MINOR);
            exit(-1);
        }

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glfwSwapInterval(swap_interval);

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();

        ImGui_ImplGlfw_InitForOpenGL(window_, true);
        const char *glsl_version = "#version 410";
        ImGui_ImplOpenGL3_Init(glsl_version);
    }
}

/**
 * @brief This starts the main event loop. 
 * 
 * @param verbose unused parameter. 
 */
void xe::Application::run(int verbose) {
    if (verbose > 0) {
        SPDLOG_INFO("{} {}", utils::get_gl_vendor(), utils::get_gl_renderer());
        SPDLOG_INFO("OpenGL {} GLSL {}", utils::get_gl_version(), utils::get_glsl_version());
    }

    auto major = utils::get_gl_version_major();
    auto minor = utils::get_gl_version_minor();

    if (major < 4 || minor < 5) {
        SPDLOG_WARN("OpenGL version {}.{} is not supported. Minimum required version is 4.5", major, minor);
    }

    int flags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
        SPDLOG_INFO("OpenGL context has debug flag enabled");
        if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
            setup_debug_output();
        }
    }

    init();

    loop();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    cleanup();
    glfwTerminate();
}

char *convert(const std::string &s) {
    char *pc = new char[s.size() + 1];
    std::strcpy(pc, s.c_str());
    return pc;
}

void xe::Application::run_cli(int argc, char **argv) {
    int verbose = 0;
    cxxopts::Options options("xe::Application", "Simple OpenGL Application");
    options.add_options()("v,verbose", "Verbose output",
                          cxxopts::value<int>()->default_value("1"));

    options.allow_unrecognised_options();
    auto result = options.parse(argc, argv);
    auto unmatched = result.unmatched();
    std::vector<char *> vc;
    vc.push_back(argv[0]);
    std::transform(unmatched.begin(), unmatched.end(), std::back_inserter(vc), convert);


    verbose = result["verbose"].as<int>();

    if (verbose > 0) {
        SPDLOG_INFO("{} {}", utils::get_gl_vendor(), utils::get_gl_renderer());
        SPDLOG_INFO("OpenGL {} GLSL {}", utils::get_gl_version(), utils::get_glsl_version());
    }

    init_cli(vc.size(), vc.data());
    init();

    loop();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    cleanup();
    glfwTerminate();
}

void xe::Application::loop() {
#ifdef __APPLE__
    auto macMoved = false;
#endif

    while (!glfwWindowShouldClose(window_)) {
        //Clear the framebuffer by filling it with color set using the glClearColor function.
        //Also clears the depth buffer.
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //This method should be overridden by you and will contain the rendering code.

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        frame();
        /* Swap front and back buffers
           The rendering is done into the BACK buffer, swapping it with front buffer displays it on the screen.
           This is done after n screen updates where n is the number set by the glfwSwapInterwal.
           Setting it to one as I did set the swap rate to v-sync rate.
        */
        ImGuiIO &io = ImGui::GetIO();
        ImGuiWindowFlags window_flags =
                ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
                ImGuiWindowFlags_NoSavedSettings |
                ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoMove;
        ImGui::SetNextWindowBgAlpha(0.35f);
        ImGui::SetNextWindowPos(ImVec2(10.0f, 10.0f), ImGuiCond_Always, ImVec2(0.0, 0.0));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::Begin("Hello, world!", nullptr,
                     window_flags);                          // Create a window called "Hello, world!" and append into it.

        ImGui::Text("FPS: %.1f", io.Framerate);
        ImGui::End();
        ImGui::PopStyleVar();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
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
}

void xe::Application::glfw_framebuffer_size_callback(GLFWwindow *window_ptr, int w, int h) {
    auto app_ptr = reinterpret_cast<Application *>(glfwGetWindowUserPointer(window_ptr));
    if (app_ptr) {
        app_ptr->framebuffer_resize_callback(w, h);
    }
}

void xe::Application::glfw_scroll_callback(GLFWwindow *window_ptr, double xoffset, double yoffset) {
    auto app_ptr = reinterpret_cast<Application *>(glfwGetWindowUserPointer(window_ptr));
    if (app_ptr) {
        app_ptr->scroll_callback(xoffset, yoffset);
    }
}

void xe::Application::glfw_cursor_position_callback(GLFWwindow *window, double x, double y) {
    auto app_ptr = reinterpret_cast<Application *>(glfwGetWindowUserPointer(window));
    if (app_ptr) {
        app_ptr->cursor_position_callback(x, y);
    }
}

void xe::Application::glfw_mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
    auto app_ptr = reinterpret_cast<Application *>(glfwGetWindowUserPointer(window));
    if (app_ptr) {
        app_ptr->mouse_button_callback(button, action, mods);
    }
}

void xe::Application::glfw_key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, 1);
    }
    auto app_ptr = reinterpret_cast<Application *>(glfwGetWindowUserPointer(window));
    if (app_ptr) {
        app_ptr->key_callback(key, scancode, action, mods);
    }
}

void xe::Application::key_callback(int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_S && action == GLFW_PRESS) {
        save_frame_buffer();
    }
}

void xe::Application::glfw_window_refresh_callback(GLFWwindow *window) {
    auto app_ptr = reinterpret_cast<Application *>(glfwGetWindowUserPointer(window));
    if (app_ptr) {
        app_ptr->window_refresh_callback();
    }
}

void xe::Application::save_frame_buffer() {
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glReadBuffer(GL_FRONT);
    if (glGetError() == GL_INVALID_OPERATION)
        SPDLOG_WARN("Saving Frame buffer error: Front buffer does not exist.");

    auto [w, h] = frame_buffer_size();
    auto data = (GLubyte *) malloc(w * h * 3);
    OGL_CALL(glReadPixels(0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, data));

    stbi_flip_vertically_on_write(1);
    std::stringstream ss;
    ss << "screenshot_" << screenshot_n_ << ".png";
    spdlog::info("Saving screenshot to {}", ss.str());
    stbi_write_png(ss.str().c_str(), w, h, 3, data, w * 3);
    ++screenshot_n_;
}
