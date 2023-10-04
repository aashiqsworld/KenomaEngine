#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

using glm::vec3;

struct Transform {
    vec3 position   = vec3(0, 0, 0);
    vec3 rotation   = vec3(0, 0, 0);
    vec3 scale      = vec3(1, 1, 1);
};

/**
 * Base class for all objects that exist in the scene
 */
class GameObject {
public:
    virtual ~GameObject();
    Transform transform;
    glm::mat4 GetTransformationMatrix() const;
    void Rotate(float x, float y, float z);
    void Rotate(vec3 rotation);
    void Translate(vec3 translation);
    void Translate(float x, float y, float z);
    void Scale(vec3 scale);
    void Scale(float x, float y, float z);
};