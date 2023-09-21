cmake_minimum_required(VERSION 3.15)
project(Triangle)

add_compile_definitions(PROJECT_NAME="${PROJECT_NAME}" PROJECT_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

add_executable(${PROJECT_NAME}
        main.cpp
        app.h
        app.cpp
        )
        
target_link_libraries(${PROJECT_NAME} PUBLIC spdlog::spdlog)


