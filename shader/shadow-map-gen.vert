#version 410 core
layout (location = 0) in vec3 aPos;

/* projection, view for light computation */
uniform mat4 lightSpaceTransform;

uniform mat4 model;

void main() {
    gl_Position = lightSpaceTransform * model * vec4(aPos, 1.0);
}