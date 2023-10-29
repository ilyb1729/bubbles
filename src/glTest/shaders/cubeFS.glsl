#version 460 core

layout(location = 0) in vec3 v_color;
layout(location = 1) in vec2 tex_coord;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float s_time;

layout(location = 0) out vec4 out_color;

void main() {
    out_color = mix(texture(texture1, tex_coord), texture(texture2, tex_coord), s_time);
}