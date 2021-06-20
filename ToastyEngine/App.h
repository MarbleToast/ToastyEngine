#pragma once

#include "Window.h"
#include "Vector.h"
#include "Camera.h"
#include "Player.h"

#include "include/glm/gtc/matrix_transform.hpp"
#include <exception>
#include <stdio.h>
#include <string_view>


class Application {
	private:
		Window* window;

	public:
		Application(
			const int width,
			const int height,
			const std::string_view name,
			bool fullscreen
		):
			window(new Window(width, height, name, fullscreen))
		{

		};

		constexpr auto getWrappedWindow() const -> Window* {
			return window;
		};

		void run();
		
		~Application() {
			delete window;
			glfwTerminate();
		}
};