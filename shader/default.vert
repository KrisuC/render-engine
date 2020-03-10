#version 410 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

/* after rasterization */
out FROM_VS_TO_FS {
    vec2 texCoords;
    vec3 worldPos;
    vec3 normal;
} frag;

layout (std140) uniform GlobalTransform {
    mat4 projection;
    mat4 view;
};

uniform mat4 model;

void main() {
    frag.texCoords = aTexCoords;
    frag.worldPos = vec3(model * vec4(aPos, 1.0));
    // The scale must be uniform
    frag.normal = mat3(model) * aNormal;

    gl_Position = projection * view * vec4(frag.worldPos, 1.0);
}
