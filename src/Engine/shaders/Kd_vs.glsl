#version 420

layout (location = 0) in vec4 a_vertex_position;
layout (location = 5) in vec4 a_vertex_color;
layout (location = 3) in vec2 a_vertex_texcoord_0;

out vec4 vertex_color;
out vec2 vertex_texcoord_0;

layout (std140, binding = 1) uniform Transformations {
    mat4 PVM;
};

void main() {
    vertex_color = a_vertex_color;
    vertex_texcoord_0 = a_vertex_texcoord_0;
    gl_Position = PVM*a_vertex_position;
}
