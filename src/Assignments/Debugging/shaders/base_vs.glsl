#version 410

layout(location=0) in  vec4 a_vertex_position;

void main() {
    gl_Position = a_vertex_position;
}
