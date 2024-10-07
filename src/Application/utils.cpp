//
// Created by pbialas on 05.08.2020.
//
#include "utils.h"

#include <iostream>
#include <sstream>
#include <unordered_map>

#include "spdlog/spdlog.h"

#include "glad/gl.h"

#include "Application/shader_source.h"

namespace xe {
    namespace utils {

        std::string get_gl_version(void) {
            auto version = glGetString(GL_VERSION);
            return std::string((const char *) version);
        }

        int get_gl_version_major(void) {
            int major;
            glGetIntegerv(GL_MAJOR_VERSION, &major);
            return major;
        }

        int get_gl_version_minor(void) {
            int minor;
            glGetIntegerv(GL_MINOR_VERSION, &minor);
            return minor;
        }

        std::string get_gl_vendor(void) {
            auto vendor = glGetString(GL_VENDOR);
            return std::string((const char *) vendor);
        }

        std::string get_gl_renderer(void) {
            auto renderer = glGetString(GL_RENDERER);
            return std::string((const char *) renderer);
        }

        std::string get_glsl_version(void) {
            auto glsl_version = glGetString(GL_SHADING_LANGUAGE_VERSION);
            return std::string((const char *) glsl_version);
        }

        std::string get_gl_description(void) {
            std::stringstream ss;
            auto vendor = glGetString(GL_VENDOR);
            auto renderer = glGetString(GL_RENDERER);
            auto version = glGetString(GL_VERSION);
            auto glsl_version = glGetString(GL_SHADING_LANGUAGE_VERSION);

            int gl_major_version;
            int gl_minor_version;
            glGetIntegerv(GL_MAJOR_VERSION, &gl_major_version);
            glGetIntegerv(GL_MINOR_VERSION, &gl_minor_version);

            ss << vendor << " " << renderer << " OpenGL " << gl_major_version << "." << gl_minor_version << "\n";
            ss << version << " " << glsl_version;

            return ss.str();
        }

        std::string shader_type(GLenum type) {

            switch (type) {
                case GL_VERTEX_SHADER:
                    return "Vertex";
                case GL_FRAGMENT_SHADER:
                    return "Fragment";
#if (MAJOR >= 3) && (MINOR >= 2)
                case GL_GEOMETRY_SHADER:
                    return "Geometry";
#endif
#if (MAJOR >= 4) && (MINOR >= 3)
                case GL_COMPUTE_SHADER:
                    return "Compute";
#endif
#if (MAJOR >= 4) && (MINOR >= 3)
                case GL_TESS_CONTROL_SHADER:
                    return "Tesselation Control";
                case GL_TESS_EVALUATION_SHADER:
                    return "Tesselation Evaluation";

#endif
            }
            return "Unknown";
        }

        std::string error_msg(GLenum status) {
            switch (status) {
                case GL_INVALID_ENUM:
                    return "INVALID ENUM";
                case GL_INVALID_VALUE:
                    return "INVALID VALUE";
                case GL_INVALID_OPERATION:
                    return "INVALID OPERATION";
#if MINOR > 2
                case GL_STACK_OVERFLOW:
                    return "STACK OVERFLOW";
                case GL_STACK_UNDERFLOW:
                    return "STACK UNDERFLOW";
#endif
                case GL_OUT_OF_MEMORY:
                    return "OUT OF MEMORY";
                case GL_INVALID_FRAMEBUFFER_OPERATION:
                    return "INVALID FRAMEBUFFER OPERATION";
                default:
                    return "UNKNOWN ERROR";
            }
        }

        GLenum
        get_and_report_error(const std::string function_call, std::string file_name, int line_number, bool critical) {
            auto error = glGetError();
            auto error_name = error_msg(error);
            std::stringstream ss;
            if (error != GL_NO_ERROR) {
                ss << "OpenGL error: " << error_name;
                if (!function_call.empty()) {
                    ss << "  " << function_call;
                }
                if (!file_name.empty()) {
                    ss << " " << file_name;
                }

                if (line_number >= 0) {
                    ss << ":" << line_number;
                }
                if (critical) {
                    spdlog::critical(ss.str());
                    exit(-1);
                } else {
                    spdlog::error(ss.str());
                }
            }
            return error;
        }

        GLuint link_program(GLuint program) {
            glLinkProgram(program);
            GLint link_status;
            glGetProgramiv(program, GL_LINK_STATUS, &link_status);
            if (!link_status) {
                spdlog::error("Error linking program\n");
                GLint max_log_length = 0;
                glGetProgramiv(program, GL_INFO_LOG_LENGTH, &max_log_length);
                GLchar *info_log = new GLchar[max_log_length];
                glGetProgramInfoLog(program, max_log_length, &max_log_length, info_log);
                std::istringstream iss(info_log);
                std::string line;
                while (std::getline(iss, line)) {
                    spdlog::error(line);
                }
                return 0;
            }

            return program;
        }

        void delete_shaders(shader_map_t &shaders) {
            for (const auto &[shader_type, shader]: shaders) {
                if (shader > 0)
                    glDeleteShader(shader);
            }
        }

        GLuint create_program(const shader_source_map_t &shaders_src, const std::string &version) {
            shader_map_t shaders;
            for (const auto &[shader_type, src]: shaders_src) {
                auto shader = create_shader_from_file(shader_type, src);
                if (shader > 0)
                    shaders[shader_type] = shader;
                else {
                    delete_shaders(shaders);
                    return 0;
                }
            }

            GLuint program = glCreateProgram();
            if (program == 0) {
                spdlog::error("Error creating program");
                delete_shaders(shaders);
                return 0;
            }
            for (const auto &[shader_type, shader]: shaders) {
                glAttachShader(program, shader);
                auto status = glGetError();
                if (status != GL_NO_ERROR) {
                    spdlog::error("Error attaching {} shader: {}", utils::shader_type(shader_type), error_msg(status));
                    glDeleteProgram(program);
                    delete_shaders(shaders);
                    return 0;
                }
            }
            if (link_program(program) == 0) {
                spdlog::error("Cannot link program");
                glDeleteProgram(program);
                delete_shaders(shaders);
                return 0;
            }
            return program;
        }

        GLuint create_program(const shader_source_map_t &shaders_src) {
            return create_program(shaders_src, "");
        }

        GLuint create_shader_from_source(GLenum type, source_t &shader_source) {

#ifdef __APPLE__
            shader_source.replace_version("410");
#endif

            auto shader = glCreateShader(type);
            if (shader == 0) {
                spdlog::error("Error creating {} shader", utils::shader_type(type));
                return 0;
            }

            if (shader_source.size() == 0) return 0;


            glShaderSource(shader, shader_source.size(), shader_source.data(), nullptr);

            glCompileShader(shader);
            GLint is_compiled = 0;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &is_compiled);
            if (!is_compiled) {
                GLint max_log_length = 0u;
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &max_log_length);
                std::string error_log;
                error_log.resize(max_log_length);
                int length = 0;
                glGetShaderInfoLog(shader, max_log_length, &length, error_log.data());
                glDeleteShader(shader);


                spdlog::error("Error compiling {} shader", shader_type(type));
                std::istringstream iss(error_log.substr(0, length));
                std::string line;
                while (std::getline(iss, line)) {
                    spdlog::error(line);
                }

                return 0;
            }
            return shader;
        }


        GLuint create_shader_from_file(GLenum type, const std::string &path) {
            source_t shader_source;
            shader_source.load(path);

            if (shader_source.size() == 0)
                return 0;
#ifdef __APPLE__
            shader_source.replace_version("410");
#endif

            return create_shader_from_source(type, shader_source);
        }
    }
}
