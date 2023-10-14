#include "GameObject.hpp"
#include "glm/ext/matrix_transform.hpp"

GameObject::~GameObject() {

}

glm::mat4 GameObject::GetTransformationMatrix() const {
    auto t = glm::mat4(1.0f);
    t = glm::rotate(t, glm::radians(transform.rotation.x), glm::vec3(1.0, 0.0, 0.0));
    t = glm::rotate(t, glm::radians(transform.rotation.y), glm::vec3(0.0, 1.0, 0.0));
    t = glm::rotate(t, glm::radians(transform.rotation.z), glm::vec3(0.0, 0.0, 1.0));
    t = glm::translate(t, transform.position);
    t = glm::scale(t, transform.scale);

    return t;
}

void GameObject::Rotate(float x, float y, float z) {
    transform.rotation.x += x;
    transform.rotation.y += y;
    transform.rotation.z += z;
}

void GameObject::Rotate(vec3 rotation) {
    transform.rotation.x += rotation.x;
    transform.rotation.y += rotation.y;
    transform.rotation.z += rotation.z;
}

void GameObject::Translate(vec3 translation) {
    transform.position.x += translation.x;
    transform.position.y += translation.y;
    transform.position.z += translation.z;
}

void GameObject::Translate(float x, float y, float z) {
    transform.position.x += x;
    transform.position.y += y;
    transform.position.z += z;
}

void GameObject::Scale(vec3 scale) {
    transform.scale.x = scale.x;
    transform.scale.y = scale.y;
    transform.scale.z = scale.z;
}

void GameObject::Scale(float x, float y, float z) {
    transform.scale.x = x;
    transform.scale.y = y;
    transform.scale.z = z;
}










