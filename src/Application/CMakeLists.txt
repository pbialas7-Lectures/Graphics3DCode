cmake_minimum_required(VERSION 3.15)
project(application CXX)

file(GLOB IMGUI_SRC ${IMGUI_DIR}/*.cpp)

add_library(${PROJECT_NAME}
        application.cpp
        application.h
        utils.cpp
        utils.h
        shader_source.cpp
        shader_source.h
        RegisteredObject.cpp
        RegisteredObject.h
        stb.cpp
        uniforms.h
        uniforms.cpp
        ${IMGUI_DIR}/imgui.h
        ${IMGUI_SRC}
        ${IMGUI_DIR}/backends/imgui_impl_glfw.h
        ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
        ${IMGUI_DIR}/backends/imgui_impl_opengl3.h
        ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
        debug.h
        debug.cpp
)

message(${IMGUI_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${IMGUI_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog cxxopts)