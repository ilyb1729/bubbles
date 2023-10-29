#version 460 core

uniform vec3 light_color;
uniform vec2 object_color;

layout(location = 0) out vec4 out_color;

void main() {
    out_color = vec4(1.0);   
}