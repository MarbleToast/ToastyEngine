#include "GameObject.h"

GameObject::GameObject(const std::string_view id, Vec3 pos): identifier(id) {
	transform.position = pos;
    transform.rotation = { DEFAULT_YAW, DEFAULT_PITCH };
	front = Vec3(0.0f, 0.0f, -1.0f);

	updateVectors();
}

void GameObject::update() {
    if (parent) {
        transform.position = parent->transform.position;
        parent->transform.rotation = transform.rotation;
    }
    updateVectors();
}

void GameObject::handleInput() {}

void GameObject::updateVectors() {
    Vec3 temp;
    temp.x = cos(glm::radians(transform.rotation.x)) * cos(glm::radians(transform.rotation.y));
    temp.y = sin(glm::radians(transform.rotation.y));
    temp.z = sin(glm::radians(transform.rotation.x)) * cos(glm::radians(transform.rotation.y));

    front = glm::normalize(temp);
    right = glm::normalize(glm::cross(worldUp, front));
    up = glm::cross(front, right);
}

void GameObject::setParent(GameObject& obj) {
    parent = &obj;
}

constexpr GameObject* GameObject::getParent() const {
    return parent;
}
