# External Dependencies

## GLFW

From the [GLFW website](https://www.glfw.org/docs/latest/index.html):

> GLFW is a free, Open Source, multi-platform library for OpenGL, OpenGL ES and Vulkan application development. It
> provides a simple, platform-independent API for creating windows, contexts and surfaces, reading input, handling
> events, etc.

## GLAD

Vulkan/GL/GLES/EGL/GLX/WGL Loader-Generator based on the official specifications for multiple languages.

## GLM

> OpenGL Mathematics (GLM) is a header only C++ mathematics library for graphics software based on the OpenGL Shading
> Language (GLSL) specifications.

## spdlog

> Very fast, header-only/compiled, C++ logging library.

## stb

## tinyobjloader

> Tiny but powerful single file wavefront obj loader written in C++03. No dependency except for C++ STL. It can parse
> over 10M polygons with moderate memory and time.
## RenderDoc

> RenderDoc is a free MIT licensed stand-alone graphics debugger that allows quick and easy single-frame capture and
> detailed introspection of any application using Vulkan, D3D11, OpenGL & OpenGL ES or D3D12.

Only the in-application API header `renderdoc_app.h` is included. When an application is started under RenderDoc
on Linux, pressing Ctrl-F captures the next frame.
