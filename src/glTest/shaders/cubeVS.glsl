#version 460 core

layout(location = 0) in vec3 i_position;
layout(location = 2) in vec3 i_color;
layout(location = 1) in vec2 i_tex_coord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

layout(location = 0) out vec3 v_color;
layout(location = 1) out vec2 tex_coord;

void main() {
    gl_Position = projection * view * model * vec4(i_position, 1.0);
    tex_coord = i_tex_coord;
    v_color = i_color;
}