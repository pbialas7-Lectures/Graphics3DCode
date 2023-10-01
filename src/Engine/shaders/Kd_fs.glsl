#version 420

layout(location=0) out vec4 vFragColor;
in vec4 vertex_color;
in vec2 vertex_texcoord_0;

layout(std140, binding=0) uniform Material {
    vec4 Kd;
    bool use_vertex_colors;
    bool use_map_Kd;
};

uniform sampler2D map_Kd;

void main() {
    vec4 color = vec4(1.0, 1.0, 1.0, 1.0);
    if (use_vertex_colors)
    color = vertex_color;

    color*=Kd;

    if (use_map_Kd)
    color*=texture(map_Kd, vertex_texcoord_0);

    vFragColor = color;


}
