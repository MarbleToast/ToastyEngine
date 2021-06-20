#pragma once
#include "GameObject.h"
#include "Matrix.h"

const float DEFAULT_FOV = 90.f;
const float DEFAULT_SENSITIVITY = .5f;

class Camera : public GameObject {
    
    private:
        static inline Camera* currentCamera = nullptr;
        Vec3 target;

    public:
        Camera(Vec3 pos);
        static constexpr auto getCurrentCamera() -> Camera* {
            return currentCamera;
        };

        Mat4 getViewMatrix();
        virtual void handleInput() override {};
        

        void handleMouseMovement(double xOffset, double yOffset, GLboolean constrainPitch = true);
        void setParent(GameObject& parent);

        float fOV;
        float sensitivity;
};

