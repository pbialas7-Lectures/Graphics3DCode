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
#include <vector>

#include "spdlog/spdlog.h"
#include "glad/gl.h"
#include "cxxopts.hpp"

#include "utils.h"
#include "debug.h"

#include "stb/stb_image_write.h"

#if defined(__linux__)
#define XE_RENDERDOC_SUPPORTED 1
#include <dlfcn.h>
#include "RenderDoc/renderdoc_app.h"
#endif

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
 * @param debug specify if an OpenGL debug context should be created and debug output reported.
 *              Additionally, if compiled with debug version of glad, enables error checking after each OpenGL function call.
 */
xe::Application::Application(int width, int height, std::string title, bool debug, int swap_interval)
        : screenshot_n_(0) {
    SPDLOG_INFO("Application::Application(window size = {}x{}, {}, debug = {}, swap interval = {})", width, height,
                title, debug, swap_interval);

    int glfw_major, glfw_minor, glfw_revision;
    glfwGetVersion(&glfw_major, &glfw_minor, &glfw_revision);


    if (glfwInit()) {

        SPDLOG_INFO("GLFW version {}.{}.{} platform = {}", glfw_major, glfw_minor, glfw_revision,
                    xe::utils::glfw::platform_name(glfwGetPlatform()));

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, MAJOR);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, MINOR);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
        glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, debug ? GLFW_TRUE : GLFW_FALSE);

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

        init_renderdoc();

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glfwSwapInterval(swap_interval);

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();

        ImGui_ImplGlfw_InitForOpenGL(window_, true);
        const char *glsl_version = "#version 410";
        ImGui_ImplOpenGL3_Init(glsl_version);
    } else {
        SPDLOG_CRITICAL("Cannot initialize GLFW");
        exit(-1);
    }
}

/**
 * @brief This starts the main event loop. 
 * 
 * @param verbose if greater than zero, OpenGL vendor, renderer and version information is printed.
 */
void xe::Application::run(int verbose) {
    startup(verbose);
    init();
    loop();
    shutdown();
}

/**
 * @brief Reports OpenGL information and sets up debug output if the context supports it.
 */
void xe::Application::startup(int verbose) {
    if (verbose > 0) {
        SPDLOG_INFO("{} {}", utils::get_gl_vendor(), utils::get_gl_renderer());
        SPDLOG_INFO("OpenGL {} GLSL {}", utils::get_gl_version(), utils::get_glsl_version());
    }


    auto major = utils::get_gl_version_major();
    auto minor = utils::get_gl_version_minor();

    if (major < 4 || (major == 4 && minor < 5)) {
        SPDLOG_WARN("OpenGL version {}.{} is not supported. Minimum required version is 4.5", major, minor);
    }

    int flags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
        SPDLOG_INFO("OpenGL context has debug flag enabled");
        setup_debug_output();
    }
}

void xe::Application::shutdown() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    cleanup();
    glfwTerminate();
}

void xe::Application::run_cli(int argc, char **argv) {
    int verbose = 0;
    cxxopts::Options options("xe::Application", "Simple OpenGL Application");
    options.add_options()("v,verbose", "Verbose output",
                          cxxopts::value<int>()->default_value("0")->implicit_value("1"));

    options.allow_unrecognised_options();
    auto result = options.parse(argc, argv);
    auto unmatched = result.unmatched();
    std::vector<char *> vc;
    vc.push_back(argv[0]);
    for (auto &arg: unmatched) {
        vc.push_back(arg.data());
    }


    verbose = result["verbose"].as<int>();

    startup(verbose);
    init_cli(vc.size(), vc.data());
    init();
    loop();
    shutdown();
}

void xe::Application::loop() {
    while (!glfwWindowShouldClose(window_)) {
        // If a capture was requested (Ctrl-F), start it now, before any GL commands
        // for this frame are issued, so that the whole frame is captured.
        if (renderdoc_capture_requested_) {
            renderdoc_capture_requested_ = false;
            renderdoc_start_capture();
        }

        // Clears the framebuffer by filling it with color set using the glClearColor function.
        // Also clears the depth buffer.
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        //This method should be overridden by you and will contain the rendering code.
        frame();

        // Screenshot requested with Ctrl-S: save the frame before the ImGui overlay is drawn.
        if (screenshot_requested_) {
            screenshot_requested_ = false;
            save_frame_buffer();
        }

        ImGuiIO &io = ImGui::GetIO();
        ImGuiWindowFlags window_flags =
                ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
                ImGuiWindowFlags_NoSavedSettings |
                ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoMove;
        ImGui::SetNextWindowBgAlpha(0.35f);
        ImGui::SetNextWindowPos(ImVec2(10.0f, 10.0f), ImGuiCond_Always, ImVec2(0.0, 0.0));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::Begin("Info", nullptr,
                     window_flags);

        ImGui::Text("FPS: %.1f", io.Framerate);
        imgui_info();
        ImGui::End();
        ImGui::PopStyleVar();

        imgui();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        /* Swap front and back buffers
           The rendering is done into the BACK buffer, swapping it with front buffer displays it on the screen.
           This is done after n screen updates where n is the number set by the glfwSwapInterwal.
           Setting it to one as I did set the swap rate to v-sync rate.
           Setting it to zero disables v-sync.
        */
        glfwSwapBuffers(window_);

        // End the capture right after presenting, so it covers exactly one full frame.
        if (renderdoc_capturing_) {
            renderdoc_end_capture();
        }

        /* Poll for and process events */
        glfwPollEvents();
    }
}

void xe::Application::glfw_framebuffer_size_callback(GLFWwindow *window_ptr, int w, int h) {
    auto app_ptr = reinterpret_cast<Application *>(glfwGetWindowUserPointer(window_ptr));
    if (app_ptr) {
        app_ptr->framebuffer_resize_callback(w, h);
    }
}

// Mouse and keyboard events are not passed to the application while ImGui uses them, e.g. when dragging a slider
// or typing into a text field. Releases are always passed, so the application does not miss the end of a drag
// or a key press that started outside ImGui.

void xe::Application::glfw_scroll_callback(GLFWwindow *window_ptr, double xoffset, double yoffset) {
    if (ImGui::GetIO().WantCaptureMouse) {
        return;
    }
    auto app_ptr = reinterpret_cast<Application *>(glfwGetWindowUserPointer(window_ptr));
    if (app_ptr) {
        app_ptr->scroll_callback(xoffset, yoffset);
    }
}

void xe::Application::glfw_cursor_position_callback(GLFWwindow *window, double x, double y) {
    if (ImGui::GetIO().WantCaptureMouse) {
        return;
    }
    auto app_ptr = reinterpret_cast<Application *>(glfwGetWindowUserPointer(window));
    if (app_ptr) {
        app_ptr->cursor_position_callback(x, y);
    }
}

void xe::Application::glfw_mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
    if (action != GLFW_RELEASE && ImGui::GetIO().WantCaptureMouse) {
        return;
    }
    auto app_ptr = reinterpret_cast<Application *>(glfwGetWindowUserPointer(window));
    if (app_ptr) {
        app_ptr->mouse_button_callback(button, action, mods);
    }
}

void xe::Application::glfw_key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    auto app_ptr = reinterpret_cast<Application *>(glfwGetWindowUserPointer(window));
    if (!app_ptr) {
        return;
    }

    // Built-in shortcuts: Ctrl-Q quits, Ctrl-S saves a screenshot, Ctrl-F triggers a RenderDoc capture.
    if ((mods & GLFW_MOD_CONTROL) != 0 && action == GLFW_PRESS) {
        switch (key) {
            case GLFW_KEY_Q:
                glfwSetWindowShouldClose(window, 1);
                return;
            case GLFW_KEY_S:
                app_ptr->screenshot_requested_ = true;
                return;
            case GLFW_KEY_F:
                app_ptr->renderdoc_capture_requested_ = true;
                return;
        }
    }

    if (action != GLFW_RELEASE && ImGui::GetIO().WantCaptureKeyboard) {
        return;
    }
    app_ptr->key_callback(key, scancode, action, mods);
}

void xe::Application::glfw_window_refresh_callback(GLFWwindow *window) {
    auto app_ptr = reinterpret_cast<Application *>(glfwGetWindowUserPointer(window));
    if (app_ptr) {
        app_ptr->window_refresh_callback();
    }
}

void xe::Application::save_frame_buffer() {
    // Save the state changed below, so the application's own settings are not affected.
    GLint read_framebuffer, read_buffer, pack_buffer, pack_alignment;
    glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &read_framebuffer);
    glGetIntegerv(GL_READ_BUFFER, &read_buffer);
    glGetIntegerv(GL_PIXEL_PACK_BUFFER_BINDING, &pack_buffer);
    glGetIntegerv(GL_PACK_ALIGNMENT, &pack_alignment);

    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    glReadBuffer(GL_BACK);
    glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);

    auto [w, h] = frame_buffer_size();
    std::vector<GLubyte> data(w * h * 3);
    OGL_CALL(glReadPixels(0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, data.data()));

    glBindFramebuffer(GL_READ_FRAMEBUFFER, read_framebuffer);
    glReadBuffer(read_buffer);
    glBindBuffer(GL_PIXEL_PACK_BUFFER, pack_buffer);
    glPixelStorei(GL_PACK_ALIGNMENT, pack_alignment);

    stbi_flip_vertically_on_write(1);
    std::stringstream ss;
    ss << "screenshot_" << screenshot_n_ << ".png";
    spdlog::info("Saving screenshot to {}", ss.str());
    stbi_write_png(ss.str().c_str(), w, h, 3, data.data(), w * 3);
    ++screenshot_n_;
}

/**
 * @brief Looks up RenderDoc's in-application API, if the application is running under RenderDoc.
 *
 * RenderDoc injects itself via LD_PRELOAD (librenderdoc.so) before main() runs, so the module is
 * already loaded in the process; we just need to find it and fetch the API entry point. Using
 * this API to explicitly start/end a capture (Ctrl-F, see glfw_key_callback and loop()) works
 * even when RenderDoc's automatic window/swapchain association fails (a known issue with some
 * combinations of GLX and the proprietary NVIDIA driver, where the hotkey/UI "trigger capture"
 * silently does nothing): the GL driver's StartFrameCapture/EndFrameCapture only need a valid
 * device pointer to work, regardless of the window handle. Passing nullptr/nullptr for device and
 * window uses RenderDoc's "current device and window" default, which resolves correctly.
 */
void xe::Application::init_renderdoc() {
#if defined(XE_RENDERDOC_SUPPORTED)
    void *renderdoc_module = dlopen("librenderdoc.so", RTLD_NOW | RTLD_NOLOAD);
    if (renderdoc_module) {
        auto RENDERDOC_GetAPI =
                (pRENDERDOC_GetAPI) dlsym(renderdoc_module, "RENDERDOC_GetAPI");
        if (RENDERDOC_GetAPI) {
            int ok = RENDERDOC_GetAPI(eRENDERDOC_API_Version_1_6_0, &renderdoc_api_);
            if (ok) {
                SPDLOG_INFO("RenderDoc detected: Ctrl-F will trigger a frame capture");
            } else {
                SPDLOG_WARN("RenderDoc detected but RENDERDOC_GetAPI failed");
                renderdoc_api_ = nullptr;
            }
        }
    }
#endif
}

void xe::Application::renderdoc_start_capture() {
#if defined(XE_RENDERDOC_SUPPORTED)
    if (renderdoc_api_) {
        auto api = reinterpret_cast<RENDERDOC_API_1_6_0 *>(renderdoc_api_);
        api->StartFrameCapture(nullptr, nullptr);
        renderdoc_capturing_ = true;
        SPDLOG_INFO("RenderDoc: capturing frame");
    } else {
        SPDLOG_WARN("RenderDoc capture requested (Ctrl-F) but RenderDoc API is not available "
                    "(run the application under RenderDoc to enable this)");
    }
#endif
}

void xe::Application::renderdoc_end_capture() {
#if defined(XE_RENDERDOC_SUPPORTED)
    if (renderdoc_api_) {
        auto api = reinterpret_cast<RENDERDOC_API_1_6_0 *>(renderdoc_api_);
        api->EndFrameCapture(nullptr, nullptr);
        SPDLOG_INFO("RenderDoc: frame capture finished");
    }
    renderdoc_capturing_ = false;
#endif
}
