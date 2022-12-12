#version 460

layout(location=0) in vec4 a_vertex_position;

#if __VERSION__ > 410
layout(std140, binding=1) uniform Transformations {
#else
    layout(std140) uniform Transformations {
    #endif
    mat4 PVM;
};



void main() {

    gl_Position =  PVM * a_vertex_position;
}
