#version 410 core

out vec4 FragColor;


out vec4 FragColor;

in FROM_VS_TO_FS {
    vec2 texCoords;
    vec3 worldPos;
    vec3 normal;
    vec4 lightSpacePos;
} frag;

const float PI = 3.14159265359;

/*********** Material Configuration **************/
uniform struct Vec3fMap { sampler2D map; vec3 value; bool use_map; }
m_albedo,
m_normal,
m_specular,
m_emissive;

uniform struct FloatMap { sampler2D map; float value; bool use_map; }
m_metallic,
m_roughness,
m_ao,
m_height;

/************* Lights Configuration **************/
const int DIRECTIONAL = 0, POINT = 1, SPOT = 2;
const int MAX_LIGHT = 20;

layout (std140) uniform LightInformation {
    struct Lights {
        vec3 position;               // 0-4N (N: 4 byte - a float)
        float cone_angle_in_radian;  // 3-4N

        vec3 direction;              // 4-8N
        int ltype;                   // 7-8N

        vec3 color;                  // 8-12N

    } lights[MAX_LIGHT];             // 0-240N [12N * MAX_LIGHT(20) == 240N]

    vec3 cameraPosition;             // 240-244N
    int lights_cnt;                  // 243-244N
// 244 * 4 < 1024 bytes
};

uniform sampler2D shadowMap;

/**********************IBL*************************/
uniform struct IBL {
    samplerCube irradiance;
    samplerCube prefilter;
    sampler2D   brdfLUT;
} ibl;

/*************************************************/


void main() {
    FragColor = vec4(0, 1, 1, 1.0);
}




