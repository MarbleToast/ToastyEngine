#pragma once

#ifndef MOUSE
#define MOUSE

#include "Camera.h"

#include <GLFW/glfw3.h>

namespace Mouse {
    inline bool mouseMovedYet = false;
    inline double lastX, lastY = 0;

    inline auto CursorMoveCallback(GLFWwindow* window, double xpos, double ypos) -> void
    {
        if (!mouseMovedYet) {
            lastX = xpos;
            lastY = ypos;
            mouseMovedYet = true;
        }

        double xoffset = xpos - lastX;
        double yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

        lastX = xpos;
        lastY = ypos;

        Camera::getCurrentCamera()->handleMouseMovement(xoffset, yoffset);
    }
};
#endif