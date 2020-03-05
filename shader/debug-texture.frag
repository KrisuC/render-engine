#version 410

out vec4 FragColor;
in  vec2 TexCoords;

uniform sampler2D map;

void main() {
    vec3 albedo = texture(map, TexCoords).rgb;
    FragColor = vec4(albedo, 1.0);
}
