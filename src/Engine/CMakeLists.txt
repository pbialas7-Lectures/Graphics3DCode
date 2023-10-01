cmake_minimum_required(VERSION 3.15)
project(Engine CXX)

add_compile_definitions(PROJECT_NAME="${PROJECT_NAME}" PROJECT_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

add_library(Engine
        Mesh.h Mesh.cpp
        Material.h
        utils.h utils.cpp
        Material.cpp
        AbstractMaterial.cpp AbstractMaterial.h KdMaterial.cpp KdMaterial.h)
target_link_libraries(Engine PUBLIC spdlog::spdlog)