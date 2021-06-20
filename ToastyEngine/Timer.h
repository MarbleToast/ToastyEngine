#pragma once
#include <GLFW/glfw3.h>

class Timer {
	static inline double deltaTime = .0;
	static inline double lastTime = .0;
public:
	static inline auto getDeltaTime() -> float {
		double currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		return deltaTime;
	}
};