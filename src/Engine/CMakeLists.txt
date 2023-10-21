cmake_minimum_required(VERSION 3.15)
project(Engine CXX)

add_compile_definitions(PROJECT_NAME="${PROJECT_NAME}" PROJECT_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/*.cpp ${PROJECT_SOURCE_DIR}/*.h)
add_library(Engine ${SRC_FILES})
target_link_libraries(Engine PUBLIC objreader spdlog::spdlog)