// General use PBR shader
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

/*************************************************/


vec3  FresnelSchlick(float cosTheta, vec3 F0);
vec3  FresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness);
float DistributionGGX(vec3 N, vec3 H, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
vec3  GetNormalFromMap();

void main() {
    /* Fetching material information */
    vec3 albedo     = m.albedo.use_map?
        texture(m.albedo.map, TexCoords).rgb :
        m.albedo.value;

    float metallic  = m.metallic.use_map?
        texture(m.metallic.map, TexCoords).r :
        m.metallic.value;

    float roughness = m.roughness.use_map?
        texture(m.roughness.map, TexCoords).r :
        m.roughness.value;

    vec3 normal     = m.normal.use_map?
        GetNormalFromMap() :
        Normal;

    float ao        = m.ao.use_map?
        texture(m.ao.map, TexCoords).r :
        1.0f;

    vec3 N = normalize(normal);
    vec3 V = normalize(cameraPosition - WorldPos);
    vec3 R = reflect(-V, N);

    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic); // for non-metal, F0 is always 0.04

    float NdotV = max(dot(N, V), 0.0);

    /* Lighting */
    vec3 Lo = vec3(0.0);
    for (int i = 0; i < lights_cnt; i++) {
        vec3  L            =   normalize(lights[i].position - WorldPos);
        vec3  H            =   normalize(V + L); // half-way vector
        float dist         =   length(lights[i].position - WorldPos);
        float attenuation  =   1.0 / (dist * dist);
        vec3  radiance     =   lights[i].color * attenuation;

        float NdotL = max(dot(N, L), 0.0);
        float HdotV = max(dot(H, V), 0.0);

        float NDF = DistributionGGX(N, H, roughness);
        float G   = GeometrySmith(N, V, L, roughness);
        vec3  F   = FresnelSchlick(HdotV, F0);

        vec3  nominator   = NDF * G * F;
        /* ------------------------------------------------ */
        float denominator = 4.0 * NdotV * NdotL;

        vec3  specular = nominator / max(denominator, 0.001);

        vec3 kS = F;               // specular term
        vec3 kD = vec3(1.0) - kS;  // defuse term
        kD *= 1.0 - metallic;      // no diffuse for metallic

        vec3 diffuse = kD * albedo / PI;

        Lo += (diffuse + specular) * radiance * NdotL;
    }
    /* IBL */
    vec3 F = FresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);

    vec3 kS = F;
    vec3 kD = 1.0 - kS;
    kD *= 1.0 - metallic;

    /* IBL - diffuse part */
    vec3 irradiance = texture(ibl.irradiance, N).rgb;
    vec3 diffuse    = irradiance * albedo;

    /* IBL - specular part */
    const float MAX_REFLECTION_LOD = 4.0;
    vec3 prefilterdColor
            = textureLod(ibl.prefilter, R, roughness*MAX_REFLECTION_LOD).rgb;
    vec2 brdf = texture(ibl.brdfLUT,
                        vec2(NdotV, roughness )).rg;
    vec3 specular = prefilterdColor * (F * brdf.x + brdf.y);

    vec3 ambient = (kD * diffuse + specular) * ao;
    vec3 color = ambient + Lo;

    /* tone mapping */
    color = color / (color + vec3(1.0)); // HDR
    color = pow(color, vec3(1.0 / 2.2)); // gamma

    FragColor = vec4(color, 1.0);
}












vec3 FresnelSchlick(float cosTheta, vec3 F0) {
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}


vec3 FresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness) {
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}


float DistributionGGX(vec3 N, vec3 H, float roughness) {
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float nom = a2;
    float denom = NdotH2 * (a2 - 1.0) + 1.0;
    denom = PI * denom * denom;

    return nom / denom;
}


float GeometrySchlickGGX(float NdotV, float roughness) {
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float nom = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}


float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}


vec3 GetNormalFromMap() {
    vec3 tangentNormal = texture(m.normal.map, TexCoords).xyz * 2.0 - 1.0;

    vec3 Q1  = dFdx(WorldPos);
    vec3 Q2  = dFdy(WorldPos);
    vec2 st1 = dFdx(TexCoords);
    vec2 st2 = dFdy(TexCoords);

    vec3 N   = normalize(Normal);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}




