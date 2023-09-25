#include "GameObject.hpp"
#include "glm/ext/matrix_transform.hpp"

GameObject::~GameObject() {

}

glm::mat4 GameObject::GetTransformationMatrix() const {
    auto t = glm::mat4(1.0f);
    t = glm::scale(t, transform.scale);
    t = glm::rotate(t, glm::radians(transform.rotation.x), glm::vec3(1.0, 0.0, 0.0));
    t = glm::rotate(t, glm::radians(transform.rotation.y), glm::vec3(0.0, 1.0, 0.0));
    t = glm::rotate(t, glm::radians(transform.rotation.z), glm::vec3(0.0, 0.0, 1.0));
    t = glm::translate(t, transform.position);
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






