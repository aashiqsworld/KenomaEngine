#version 460 core


struct Material {
    sampler2D   diffuse;
    sampler2D   specular;
    float       shininess;
};

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

layout (location = 0) out vec4 oPixel;

layout (location = 0) in vec2 iUvs;
layout (location = 1) in flat uint iBaseColorIndex;
layout (location = 2) in flat uint iNormalIndex;
layout (location = 3) in vec3 iNormal;
layout (location = 4) in vec3 iFragPos;
layout (location = 5) in vec3 iTangent;
layout (location = 6) in vec3 iBitangent;

layout (location = 2) uniform sampler2D[16] uTextures;
layout (location = 18) uniform PointLight[16] pointLights;
#define MAX_POINT_LIGHTS;

uniform DirLight dirLight;
uniform PointLight pointLight;
//uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform SpotLight spotLight;

uniform Material material;

uniform vec3 viewPos;

uint testIndex;

// method signatures
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    testIndex = iBaseColorIndex;
    mat3 TBN = mat3(iTangent, iBitangent, iNormal);

    // properties
    vec3 norm = texture(uTextures[iNormalIndex], iUvs).rgb;
//    normalize(iNormal);
    norm = normalize(norm * 2.0 - 1.0);
    norm = normalize(TBN * norm);
    vec3 viewDir = normalize(viewPos - iFragPos);

    vec3 result = vec3(0.0, 0.0, 0.0);
    result = CalcDirLight(dirLight, norm, viewDir);
    result += CalcPointLight(pointLight, norm, iFragPos, viewDir);
    result += CalcSpotLight(spotLight, norm, iFragPos, viewDir);

    oPixel = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(uTextures[testIndex], iUvs).rgb);
    vec3 diffuse = light.diffuse * diff * vec3(texture(uTextures[testIndex], iUvs).rgb);
    vec3 specular = light.specular * spec * vec3(texture(uTextures[testIndex], iUvs).rgb);
    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // combine results
    vec3 ambient = light.ambient * vec3(texture(uTextures[testIndex], iUvs).rgb);
    vec3 diffuse = light.diffuse * diff * vec3(texture(uTextures[testIndex], iUvs).rgb);
    vec3 specular = light.specular * spec * vec3(texture(uTextures[testIndex], iUvs).rgb);
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(uTextures[testIndex], iUvs).rgb);
    vec3 diffuse = light.diffuse * diff * vec3(texture(uTextures[testIndex], iUvs).rgb);
    vec3 specular = light.specular * spec * vec3(texture(uTextures[testIndex], iUvs).rgb);
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}

