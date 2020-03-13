#version 410 core

out vec4 FragColor;
in  vec2 TexCoords;

uniform sampler2D map;

void main() {
    float depthValue = texture(map, TexCoords).r;
    FragColor = vec4(vec3(depthValue), 1.0);
}
