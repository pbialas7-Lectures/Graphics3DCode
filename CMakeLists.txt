cmake_minimum_required(VERSION 3.15)

project(Graphics3DCode CXX C)

add_compile_definitions(ROOT_DIR="${PROJECT_SOURCE_DIR}")

include(FetchContent)
include(ExternalProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

set(SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)

set(3rdParty_DIR ${SOURCE_DIR}/3rdParty)
include_directories(${3rdParty_DIR})
include_directories(${SOURCE_DIR})

add_compile_definitions(XE_DEBUG)


# GLAD
set(MAJOR 4)
set(MINOR 6)
set(GLAD_DEBUG OFF CACHE BOOL "" FORCE)

if (APPLE) #Apple has deprecated OpenGL :( 
    set(MAJOR 4)
    set(MINOR 1)
endif ()

add_compile_definitions(MAJOR=${MAJOR})
add_compile_definitions(MINOR=${MINOR})
set(GLAD_DIR ${3rdParty_DIR}/glad/glad_${MAJOR}_${MINOR})
if (${GLAD_DEBUG})
    set(GLAD_DIR ${GLAD_DIR}_debug)
endif ()


include_directories(${GLAD_DIR}/glad/include)
file(GLOB_RECURSE glad_SOURCES "${GLAD_DIR}/glad/src/*.c")
add_library(glad OBJECT ${glad_SOURCES})

# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# GLFW
FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
)
FetchContent_MakeAvailable(glfw)

# GLM
FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
)
FetchContent_MakeAvailable(glm)

# SPDLOG
FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG v1.x
)
FetchContent_MakeAvailable(spdlog)

# CXXOPTS
FetchContent_Declare(
        cxxopt
        GIT_REPOSITORY https://github.com/jarro2783/cxxopts.git
)
FetchContent_MakeAvailable(cxxopt)

# IMGUI
FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui
)
FetchContent_MakeAvailable(imgui)

SET(IMGUI_DIR ${CMAKE_BINARY_DIR}/_deps/imgui-src)

find_package(OpenGL REQUIRED)

# My part

link_libraries(OpenGL::GL glfw glm glad)
add_subdirectory(src/Application)

if (EXISTS ${SOURCE_DIR}/ObjectReader)
    add_subdirectory(src/ObjectReader)
else ()
    message("Warning: directory ObjectReader does not exists.")
endif ()

if (EXISTS ${SOURCE_DIR}/Engine)
    add_subdirectory(src/Engine)
else ()
    message("Warning: directory Engine does not exists.")
endif ()


if (EXISTS ${SOURCE_DIR}/OGL)
    add_subdirectory(src/OGL)
endif ()

if (EXISTS ${SOURCE_DIR}/3rdParty/MIKKTSpace)
    add_subdirectory(src/3rdParty/MIKKTSpace)
endif ()

if (EXISTS ${SOURCE_DIR}/tests)
    add_subdirectory(src/tests)
endif ()

#Assignments
set(ASSIGNMENTS
        Debugging
        00_Triangle 01_House 02_Colors 03_Indices 04_Uniforms 05_PVM 06_Pyramid 07_Resize 08_Zoom
        09_CameraMovement 10_Mesh 11_KdMaterial 12_Textures 13_OBJReader 14_Diffuse
        15_Specular)

set(ASSIGNMENTS_DIR ${SOURCE_DIR}/Assignments)
link_libraries(application)

foreach (assignment ${ASSIGNMENTS})
    if (EXISTS ${ASSIGNMENTS_DIR}/${assignment})
        add_subdirectory(${ASSIGNMENTS_DIR}/${assignment})
    endif ()
endforeach ()
