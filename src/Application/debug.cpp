//
// Created by pbialas on 07.10.24.
//

#include <string>
#include <string_view>

#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>
#include <glad/gl.h>
#include <spdlog/spdlog.h>

namespace {
    spdlog::level::level_enum get_spdlog_level(GLenum severity) {
        switch (severity) {
            case GL_DEBUG_SEVERITY_HIGH:
                return spdlog::level::err;
            case GL_DEBUG_SEVERITY_MEDIUM:
                return spdlog::level::warn;
            case GL_DEBUG_SEVERITY_LOW:
                return spdlog::level::info;
            case GL_DEBUG_SEVERITY_NOTIFICATION:
                return spdlog::level::debug;
            default:
                return spdlog::level::warn;
        }
    }

    std::string get_debug_message_source(GLenum source) {

        switch (source) {
            case GL_DEBUG_SOURCE_API:
                return "API";
            case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
                return "Window System";
            case GL_DEBUG_SOURCE_SHADER_COMPILER:
                return "Shader Compiler";
            case GL_DEBUG_SOURCE_THIRD_PARTY:
                return "Third Party";
            case GL_DEBUG_SOURCE_APPLICATION:
                return "Application";
            case GL_DEBUG_SOURCE_OTHER:
                return "Other";
            default:
                return "Unknown";
        }
    }

    std::string get_debug_message_type(GLenum type) {
        switch (type) {
            case GL_DEBUG_TYPE_ERROR:
                return "Error";
            case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
                return "Deprecated Behaviour";
            case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
                return "Undefined Behaviour";
            case GL_DEBUG_TYPE_PORTABILITY:
                return "Portability";
            case GL_DEBUG_TYPE_PERFORMANCE:
                return "Performance";
            case GL_DEBUG_TYPE_MARKER:
                return "Marker";
            case GL_DEBUG_TYPE_PUSH_GROUP:
                return "Push Group";
            case GL_DEBUG_TYPE_POP_GROUP:
                return "Pop Group";
            case GL_DEBUG_TYPE_OTHER:
                return "Other";
            default:
                return "Unknown";
        }
    }

    std::string get_debug_message_severity(GLenum severity) {
        switch (severity) {
            case GL_DEBUG_SEVERITY_HIGH:
                return "High";
            case GL_DEBUG_SEVERITY_MEDIUM:
                return "Medium";
            case GL_DEBUG_SEVERITY_LOW:
                return "Low";
            case GL_DEBUG_SEVERITY_NOTIFICATION:
                return "Notification";
            default:
                return "Unknown";
        }
    }

    void log_lines(spdlog::level::level_enum level, std::string_view text) {
        while (true) {
            const auto newline = text.find('\n');
            auto line = text.substr(0, newline);
            if (!line.empty() && line.back() == '\r') {
                line.remove_suffix(1);
            }
            spdlog::log(level, "  {}", line);

            if (newline == std::string_view::npos) {
                break;
            }
            text.remove_prefix(newline + 1);
        }
    }

    void APIENTRY gl_debug_output_callback(GLenum source,
                                           GLenum type,
                                           unsigned int id,
                                           GLenum severity,
                                           GLsizei length,
                                           const char *pmessage,
                                           const void *userParam) {
// ignore non-significant error/warning codes
        if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

        const auto level = get_spdlog_level(severity);
        const auto message = std::string(pmessage, length);
        spdlog::log(level, "---------------");
        spdlog::log(level, "Debug message ({}):", id);
        log_lines(level, message);
        spdlog::log(level, "  Source: {}", get_debug_message_source(source));
        spdlog::log(level, "  Type: {}", get_debug_message_type(type));
        spdlog::log(level, "  Severity: {}", get_debug_message_severity(severity));
    }
}

void setup_debug_output() {
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(gl_debug_output_callback, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
}