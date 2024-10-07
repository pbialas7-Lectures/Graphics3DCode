//
// Created by pbialas on 05.08.2020.
//

#pragma once

#include <iostream>
#include <string>
#include <unordered_map>

#include "glad/gl.h"

namespace xe {
    namespace utils {

        using shader_source_map_t = std::unordered_map<GLenum, std::string>;
        using shader_map_t = std::unordered_map<GLenum, GLuint>;
        using shader_map_element_t = std::pair<GLenum, std::string>;

        std::string get_gl_version(void);

        std::string get_gl_vendor(void);

        std::string get_gl_renderer(void);

        std::string get_glsl_version(void);

        int get_gl_version_major(void);
        int get_gl_version_minor(void);

        std::string get_gl_description(void);

        // Error reporting/debuging

        std::string shader_type(GLenum type);

        std::string error_msg(GLenum status);

        GLenum
        get_and_report_error(const std::string function_call = "", std::string file_name = "", int line_number = -1,
                             bool critical = false);

        // Program creation utils

        std::string load_file(const std::string &path);

        GLuint create_shader_from_file(GLenum type, const std::string &path);

        GLuint link_program(GLuint program);

        GLuint create_program(const std::string &vs_path, const std::string &fs_path);

        GLuint create_program(const shader_source_map_t &shaders_src);

    }
}

#ifdef DEBUG_NO_ABORT
#define CRITICAL__  false
#else
#define CRITICAL__  true
#endif

#ifndef NO_OGL_CALL
#define OGL_CALL(call)                                              \
        call;                                                       \
        xe::utils::get_and_report_error(#call, __FILE__, __LINE__, CRITICAL__);
#else
#define OGL_CALL(call) {call;}
#endif
