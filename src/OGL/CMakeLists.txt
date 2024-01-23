cmake_minimum_required(VERSION 3.15)
project(OGL CXX)

add_compile_definitions(PROJECT_NAME="${PROJECT_NAME}" PROJECT_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

add_library(OGL utils.cpp utils.h)
target_link_libraries(OGL PUBLIC  spdlog::spdlog)