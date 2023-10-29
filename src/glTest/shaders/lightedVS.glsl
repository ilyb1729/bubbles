#version 460 core

layout(location = 0) in vec3 i_position;
layout(location = 1) in vec3 i_normal;
layout(location = 2) in vec2 i_tex_coord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

layout(location = 0) out vec3 normal;
layout(location = 1) out vec3 frag_position;
layout(location = 2) out vec2 tex_coord;

void main() {
    gl_Position = projection * view * model * vec4(i_position, 1.0);
    frag_position = vec3(model * vec4(i_position, 1.0));
    normal = mat3(transpose(inverse(model))) * i_normal; // (model * vec4(i_normal, 1.0)).xyz;
    tex_coord = i_tex_coord;
}