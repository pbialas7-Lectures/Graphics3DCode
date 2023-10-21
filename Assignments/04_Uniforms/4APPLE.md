# Uniform interface blocks and Apple computers

Unfortunately, Apple has depreciated OpenGL support quite a while ago, and the latest version of OpenGL supported by Apple is 4.1.

The problem is that the `binding` specification in layout for uniform interface blocks was introduced in OpenGL 4.2.
Therefore, we cannot use it on Apple computers.

The declaration of the uniform interface block in the shader should look like this:

```glsl
layout(std140) uniform Mixer {
float strength;
vec3 mix_color;
};
```

Without the `binding` specification we must use another way to bind the uniform interface block to the buffer. This is done in two steps:


1. Get the index of the uniform interface block in the shader using `glGetUniformBlockIndex` function. 
2. Assign required binding to the interface block using  `glUniformBlockBinding` function.

This can be done in the following way in the `init` method of the `SimpleShapeApplication` class:

```c++
auto u_mixer_index = glGetUniformBlockIndex(program, "Mixer");
if (u_mixer_index == GL_INVALID_INDEX) {
    SPDLOG_WARN("Cannot find Mixer uniform block in program");
} else {
    glUniformBlockBinding(program, u_mixer_index,0);
}
```

This sets the binding of the uniform interface block `Mixer` to the binding point `0`. This  is equivalent to the following declaration:

```glsl
layout(std140, binding=0) uniform Mixer {
float strength;
vec3 mix_color;
};
```