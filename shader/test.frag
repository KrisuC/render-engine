#version 410 core

out vec4 FragColor;

in vec2 TexCoords;
in vec3 WorldPos;
in vec3 Normal;

const float PI = 3.14159265359;

// for calculating specular
uniform vec3 cameraPosition;

/*********** Material Configuration **************/
uniform struct Material {
    struct { sampler2D map; vec3 value; } albedo;
    struct { sampler2D map; float value; } metallic;
    struct { sampler2D map; float value; } roughness;
    struct { sampler2D map; float value; } emissive;
    sampler2D normal;
    sampler2D ao;
    sampler2D specular;
    sampler2D height;

    struct {
        bool albedo;
        bool metallic;
        bool specular;
        bool roughness;
        bool emissive;
        bool normal;
    } map_using_status;
} m;


/**********************IBL*************************/
uniform samplerCube irradiance_map;
uniform samplerCube prefilter_map;
uniform sampler2D   brdfLUT_map;



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
uniform int light_cnt = 0;

/************************************************/





void main() {
    FragColor = vec4(m.albedo.value, 1.0);
}




