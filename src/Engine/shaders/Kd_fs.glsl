#version 420

vec3 srgb_gamma_correction(vec3 color) {
    color = clamp(color, 0.0, 1.0);
    color = mix(color * 12.92, (1.055 * pow(color, vec3(1.0 / 2.4))) - 0.055, step(0.0031308, color));
    return color;
}

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

    vFragColor.a = color.a;
    vFragColor.rgb = srgb_gamma_correction(color.rgb);

}
