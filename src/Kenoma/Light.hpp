#pragma once

#include <glm/vec3.hpp>
#include "GameObject.hpp"

using glm::vec3;

struct DirectionalLight {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct SpotLight {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float cutoff;
    float outerCutoff;
    float constant;
    float linear;
    float quadratic;
};

enum LightType
{
    Directional,
    Point,
    Spot
};

class Light : public GameObject
{
public:
    Light();
    explicit Light(LightType type);
    Light(DirectionalLight directionalLight);
    Light(PointLight pointLight);
    Light(SpotLight spotLight);


};