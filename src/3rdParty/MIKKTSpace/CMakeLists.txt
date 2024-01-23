cmake_minimum_required(VERSION 3.15)
project(MIKKTSpace CXX)

add_library(mikktspace
        mikktspace.h mikktspace.c)

target_link_libraries(mikktspace PRIVATE spdlog::spdlog)