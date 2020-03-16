#version 410 core
layout (triangles) in; // A plane
layout (triangle_strip, max_vertices=18) out;

uniform mat4 shadowTransformPV[6];

out vec4 FragPos;

void main() {
    /* for each face, transform triangle to  */
    for (int face = 0; face < 6; face++) {
        gl_Layer = face;
        for (int i = 0; i < 3; i++) {
            FragPos = gl_in[i].gl_Position;
            gl_Position = shadowTransformPV[face] * FragPos;
            EmitVertex();
        }
        EndPrimitive();
    }
}