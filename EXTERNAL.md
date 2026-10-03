# External Dependencies

The project uses the following external libraries. Some are downloaded by CMake when the project is configured, at the
versions pinned in the top `CMakeLists.txt`; the others are included in the `src/3rdParty` folder.

| Library                                    | Used for                                   | Version   | Source                     | License                        |
|--------------------------------------------|--------------------------------------------|-----------|----------------------------|--------------------------------|
| [GLFW](#glfw)                              | window, OpenGL context, keyboard and mouse | 3.5.1     | downloaded                 | zlib                           |
| [GLAD](#glad)                              | loading the OpenGL functions               | 2.0       | `src/3rdParty/glad`        | (WTFPL or CC0-1.0) and Apache-2.0 |
| [GLM](#glm)                                | vectors, matrices and transformations      | 1.0.3     | downloaded                 | MIT or Happy Bunny             |
| [spdlog](#spdlog)                          | logging messages                           | 1.17.0    | downloaded                 | MIT                            |
| [cxxopts](#cxxopts)                        | command line options                       | 3.3.1     | downloaded                 | MIT                            |
| [Dear ImGui](#dear-imgui)                  | user interface drawn in the window         | 1.92.9b   | downloaded                 | MIT                            |
| [stb](#stb)                                | reading and writing images                 | see below | `src/3rdParty/stb`         | MIT or public domain           |
| [tinyobjloader](#tinyobjloader)            | reading OBJ models                         | 2.0       | `src/3rdParty/tinyobjloader` | MIT (earcut: ISC)            |
| [MikkTSpace](#mikktspace)                  | computing tangents for normal mapping      |           | `src/3rdParty/MIKKTSpace`  | zlib-style                     |
| [RenderDoc](#renderdoc)                    | frame capture API                          | API 1.6.0 | `src/3rdParty/RenderDoc`   | MIT                            |

To update a downloaded library, change its `GIT_TAG` in the top `CMakeLists.txt` and configure the project from
scratch.

## GLFW

[GLFW](https://www.glfw.org/) creates the window with the OpenGL context and reports the keyboard and mouse input.
From the [GLFW website](https://www.glfw.org/docs/latest/index.html):

> GLFW is a free, Open Source, multi-platform library for OpenGL, OpenGL ES and Vulkan application development. It
> provides a simple, platform-independent API for creating windows, contexts and surfaces, reading input, handling
> events, etc.

## GLAD

The addresses of the OpenGL functions have to be loaded at run time. This is done by the loader generated
with [GLAD](https://github.com/Dav1dde/glad), a Vulkan/GL/GLES/EGL/GLX/WGL loader-generator based on the official
specifications. The `src/3rdParty/glad` folder contains loaders for the OpenGL 4.1, 4.3, 4.5 and 4.6 core profiles;
the one matching `MAJOR` and `MINOR` in the top `CMakeLists.txt` is used. The `_debug` variants, used when `GLAD_DEBUG`
is set, can call a function after every OpenGL call, e.g. to check for errors.

## GLM

[GLM](https://github.com/g-truc/glm) provides the vectors, matrices and transformations:

> OpenGL Mathematics (GLM) is a header only C++ mathematics library for graphics software based on the OpenGL Shading
> Language (GLSL) specifications.

## spdlog

[spdlog](https://github.com/gabime/spdlog) prints the messages on the console, including the OpenGL errors and debug
messages:

> Very fast, header-only/compiled, C++ logging library.

## cxxopts

[cxxopts](https://github.com/jarro2783/cxxopts) parses the command line options of programs started with
`Application::run_cli`.

## Dear ImGui

[Dear ImGui](https://github.com/ocornut/imgui) draws the user interface inside the window: the "Info" panel and any
windows created in `Application::imgui()`.

## stb

From the [stb](https://github.com/nothings/stb) single-file libraries the project uses `stb_image.h` (version 2.27)
to read the textures and `stb_image_write.h` (version 1.16) to save the screenshots.

## tinyobjloader

[tinyobjloader](https://github.com/tinyobjloader/tinyobjloader) reads the models in the Wavefront OBJ format:

> Tiny but powerful single file wavefront obj loader written in C++03. No dependency except for C++ STL. It can parse
> over 10M polygons with moderate memory and time.

The `mapbox/earcut.hpp` header from [earcut.hpp](https://github.com/mapbox/earcut.hpp) is used by tinyobjloader to
triangulate polygons.

## MikkTSpace

[MikkTSpace](https://github.com/mmikk/MikkTSpace) is the common standard for tangent space used by tools that bake
normal maps. It is linked to the OBJ reader library, so it can be used to compute the tangents needed for normal
mapping when they are not present in the model.

## RenderDoc

[RenderDoc](https://renderdoc.org/):

> RenderDoc is a free MIT licensed stand-alone graphics debugger that allows quick and easy single-frame capture and
> detailed introspection of any application using Vulkan, D3D11, OpenGL & OpenGL ES or D3D12.

Only the in-application API header `renderdoc_app.h` is included. Normally a frame is captured with RenderDoc's
capture key, F12 by default. On some machines, e.g. on Linux with recent NVIDIA drivers, this key does nothing; as a
workaround, pressing Ctrl-F in a program started from RenderDoc captures the next frame through this API (Linux only).
See [DEBUGGING.md](Assignments/DEBUGGING.md#renderdoc).
