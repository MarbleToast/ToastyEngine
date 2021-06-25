#pragma once
#include "GameObject.h"
#include "Matrix.h"
#include "Window.h"

const float DEFAULT_FOV = 90.f;
const float DEFAULT_SENSITIVITY = .5f;

class Camera : public GameObject {
    
    private:
        static inline Camera* currentCamera = nullptr;
        Vec3 target;
        Mat4 frustrum;

        void generateFrustrum(Window* window);

    public:
        Camera(Vec3 pos, Window* window);
        static constexpr auto getCurrentCamera() -> Camera* {
            return currentCamera;
        };

        Mat4 getViewMatrix();

        constexpr Mat4 getFrustrum() const {
            return frustrum;
        };
        virtual void handleInput() override {};
        

        void handleMouseMovement(double xOffset, double yOffset, GLboolean constrainPitch = true);
        void setParent(GameObject& parent);

        float fOV;
        float sensitivity;
};

