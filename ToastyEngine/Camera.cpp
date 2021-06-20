#include "Camera.h"
#include "include/glm/gtc/matrix_transform.hpp"

Camera::Camera(Vec3 pos): 
    fOV(DEFAULT_FOV),
    sensitivity(DEFAULT_SENSITIVITY),
    GameObject("Camera", pos)
{
    target = Vec3(0.f, 0.f, 0.f);
    updateVectors();
    currentCamera = this;
}

Mat4 Camera::getViewMatrix() {
    return glm::lookAt(transform.position, transform.position + front, up);
}

void Camera::handleMouseMovement(double xOffset, double yOffset, GLboolean constrainPitch) {
    xOffset *= sensitivity;
    yOffset *= sensitivity;

    transform.rotation += Vec2(xOffset, yOffset);

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        if (transform.rotation.y > 89.0f)
            transform.rotation.y = 89.0f;
        if (transform.rotation.y < -89.0f)
            transform.rotation.y = -89.0f;
    }

    updateVectors();
}

void Camera::setParent(GameObject& p) {
    parent = &p;
}
