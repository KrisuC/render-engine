// General use PBR shader
#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 WorldPos;
in vec3 Normal;

const int MAX_POINT_LIGHT = 16;
const int MAX_DIRECTIONAL_LIGHT = 4;
const int MAX_SPOT_LIGHT = 16;
const float PI = 3.14159265359;

// for calculating specular
uniform vec3 cameraPosition;

// TODO: support for height_map, emissive_map
// Sampler
uniform sampler2D albedo_map;
uniform sampler2D metallic_map;
uniform sampler2D roughness_map;
uniform sampler2D emissive_map;
uniform sampler2D normal_map;
uniform sampler2D ao_map;
uniform sampler2D specular_map;
uniform sampler2D height_map;
// Constant
uniform vec3 albedo_val;
uniform float metallic_val;
uniform float roughness_val;
uniform float emissive_val;
uniform float ao_val = 1.0;
// Status
uniform bool use_albedo_map = false;
uniform bool use_metallic_map = false;
uniform bool use_roughness_map = false;
uniform bool use_emissive_map = false;
uniform bool use_normal_map = false;

// IBL
uniform samplerCube irradiance_map;
uniform samplerCube prefilter_map;
uniform sampler2D   brdfLUT_map;

// Lights
struct PointLight {
    vec3 position;
    vec3 color;
} pointLights[MAX_POINT_LIGHT];

uniform int pointLightsCount = 0;

struct DirectionalLight {
    vec3 direction;
    vec3 color;
} directionalLights[MAX_DIRECTIONAL_LIGHT];

uniform int directionalLightsCount = 0;

struct SpotLight {
    vec3 position;
    vec3 direction;
    vec3 color;
    float cone_angle;
} spotLights[MAX_SPOT_LIGHT];

uniform int spotLightsCount = 0;


vec3 fresnelSchlick(float cosTheta, vec3 F0);
vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness);
float DistributionGGX(vec3 N, vec3 H, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);
vec3 getNormalFromMap();

void main() {
    vec3 albedo, normal;
    float metallic, roughness, ao;

    if (use_albedo_map) {
        albedo = texture(albedo_map, TexCoords).rgb;
    } else {
        albedo = albedo_val;
    }
    if (use_metallic_map) {
        metallic = texture(metallic_map, TexCoords).r;
    } else {
        metallic = metallic_val;
    }
    if (use_roughness_map) {
        roughness = texture(roughness_map, TexCoords).r;
    } else {
        roughness = roughness_val;
    }
    if (use_normal_map) {
        normal = getNormalFromMap();
    } else {
        normal = Normal;
    }
    if (use_emissive_map) {
        // ...
    }

    vec3 N = normalize(normal);
    vec3 V = normalize(cameraPosition - WorldPos);
    vec3 R = reflect(-V, N);

    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic); // for non-metal, F0 is always 0.04

    vec3 Lo = vec3(0.0);
    for (int i = 0; i < pointLightsCount; i++) {
        // Per-light raiance
        vec3 L = normalize(pointLights[i].position - WorldPos);
        vec3 H = normalize(V + L); // half-way
        float dist = length(pointLights[i].position - WorldPos);
        float attenuation = 1.0 / (dist * dist);
        vec3 radiance = pointLights.color * attenuation;

        // Cook-Torrance BRDF
        float NDF = DistributionGGX(N, H, roughness);
        float G   = GeometrySmith(N, V, L, roughness);
        vec3  F   = fresnelSchlick(max(dot(H, V), 0.0), F0);

        vec3 nominator    = NDF * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
        vec3 specular = nominator / max(denominator, 0.001);

        vec3 kS = F; // energy of light get relected
        vec3 kD = vec3(1.0) - kS; // refracted
        kD *= 1.0 - metallic;

        float NdotL = max(dot(N, L), 0.0);
        Lo += (kD * albedo / PI + specular) * radiance * NdotL; // no diffuse for metalic
    }
    // using IBL as ambient lighting
    vec3 F = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);

    vec3 kS = F;
    vec3 kD = 1.0 - kS;
    kD *= 1.0 - metallic;

    vec3 irradiance = texture(irradiance_map, N).rgb;
    vec3 diffuse    = irradiance * albedo;

    // get IBL specular part
    const float MAX_REFLECTION_LOD = 4.0;
    vec3 prefilterdColor
            = textureLod(prefilter_map, R, roughness*MAX_REFLECTION_LOD).rgb;
    vec2 brdf = texture(brdfLUT_map,
                        vec2( max(dot(N, V), 0.0), roughness )).rg;
    vec3 specular = prefilterdColor * (F * brdf.x + brdf.y);

    vec3 ambient = (kD * diffuse + specular) * ao;
    vec3 color = ambient + Lo;

    // HDR tone mapping
    color = color / (color + vec3(1.0)); // HDR
    color = pow(color, vec3(1.0 / 2.2)); // gamma

    FragColor = vec4(color, 1.0);
}


vec3 fresnelSchlick(float cosTheta, vec3 F0) {
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}


vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness) {
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


vec3 getNormalFromMap() {
    vec3 tangentNormal = texture(normal_map, TexCoords).xyz * 2.0 - 1.0;

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




