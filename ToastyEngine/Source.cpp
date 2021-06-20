#define STB_IMAGE_IMPLEMENTATION

#include "App.h"
#include "Window.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "Diagnostics.h"

#include <iostream>
#include <stb_image.h>

void preInit() {
	if (!glfwInit()) {
		Diagnostics::Throw("GLFW failed to initialise.");
	}
	stbi_set_flip_vertically_on_load(true);
}

int main() {
	try {
		preInit();

		Application app(640, 480, "GL", false);

		glfwSetErrorCallback(Diagnostics::ErrorCallback);
		glfwSetCursorPosCallback(app.getWrappedWindow()->getWindow(), Mouse::CursorMoveCallback);
		glfwSetKeyCallback(app.getWrappedWindow()->getWindow(), Keyboard::KeyboardCallback);
		
		app.run();
	}
	catch (std::exception& err) {
		std::cout << err.what() << std::endl;
		return -1;
	}

	return 0;
}