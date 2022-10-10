cmake_minimum_required(VERSION 3.15)
project(application CXX)

add_library(${PROJECT_NAME}
        application.cpp
        application.h
        utils.cpp
        utils.h
        shader_source.cpp 
        shader_source.h
        )