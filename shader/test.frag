#version 410 core

out vec4 FragColor;

in vec2 TexCoords;
in vec3 WorldPos;
in vec3 Normal;

const float PI = 3.14159265359;

/*********** for calculating V *******************/
uniform vec3 cameraPosition;


/*********** Material Configuration **************/
uniform struct Material {
    struct Vec3fMap { sampler2D map; vec3 value; bool use_map; }
    albedo,     normal,     specular,   emissive;

    struct FloatMap { sampler2D map; float value; bool use_map; }
    metallic,   roughness,  ao,         height;
} m;


/************* Lights Configuration **************/
const int DIRECTIONAL = 0, POINT = 1, SPOT = 2;
const int MAX_LIGHT = 20;

uniform struct Lights {
    vec3 position;
    vec3 direction;
    vec3 color;
    float cone_angle_in_radian;

    int ltype;
} lights[MAX_LIGHT];
uniform int lights_cnt = 0;


/**********************IBL*************************/
uniform struct IBL {
    samplerCube irradiance;
    samplerCube prefilter;
    sampler2D   brdfLUT;
} ibl;

/************************************************/





void main() {
    FragColor = vec4(0, 1, 1, 1.0);
}




