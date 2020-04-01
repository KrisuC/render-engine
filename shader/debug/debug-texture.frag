#version 410

out vec4 FragColor;
in  vec2 TexCoords;

uniform sampler2D map;

void main() {
    float albedo = texture(map, TexCoords).r;
    FragColor = vec4(vec3(albedo), 1.0);
}
