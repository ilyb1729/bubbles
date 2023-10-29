#version 460 core

struct Material {
    sampler2D diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 direction;
    float cutoff;
    float outercutoff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

layout(location = 0) in vec3 normal;
layout(location = 1) in vec3 frag_position;
layout(location = 2) in vec2 tex_coord;

uniform vec3 view_position;
uniform Material material;
uniform Light light;

layout(location = 0) out vec4 out_color;

void main() {
    vec3 light_direction = normalize(light.position - frag_position);

    float theta = dot(light_direction, normalize(-light.direction));
    float epsilon = light.cutoff - light.outercutoff;
    float intensity = clamp((theta - light.outercutoff) / epsilon, 0.0, 1.0);

    float distance = length(light.position - frag_position);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

    vec3 ambient = vec3(texture(material.diffuse, tex_coord)) * light.ambient;

    vec3 norm = normalize(normal);
    float diff = max(dot(norm, light_direction), 0.0);
    vec3 diffuse = diff * vec3(texture(material.diffuse, tex_coord)) * light.diffuse;

    vec3 view_direction = normalize(view_position - frag_position);
    vec3 reflect_direction = reflect(-light_direction, norm);
    float spec = pow(max(dot(view_direction, reflect_direction), 0.0), material.shininess);
    vec3 specular = material.specular * spec * light.specular;

    // ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    diffuse *= intensity;
    specular *= intensity;

    out_color = vec4(ambient + diffuse + specular, 1.0);

}