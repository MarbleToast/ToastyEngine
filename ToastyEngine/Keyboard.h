#pragma once

#ifndef KEYBOARD
#define KEYBOARD

#include "GameObject.h"

#include <GLFW/glfw3.h>
#include <algorithm>
#include <vector>
#include <map>

namespace Keyboard {
	inline std::vector<GameObject*> listenerStack{};
	inline std::map<int, bool> keyMap{
		{GLFW_KEY_W, false},
		{GLFW_KEY_S, false},
		{GLFW_KEY_A, false},
		{GLFW_KEY_D, false},
		{GLFW_KEY_LEFT_SHIFT, false},
		{GLFW_KEY_SPACE, false},
	};;
	inline bool keyboardActive = false;

	inline auto addListener(GameObject& obj) -> void {
		listenerStack.push_back(&obj);
	};

	inline auto removeListener(GameObject& obj) -> void {
		listenerStack.erase(std::remove(listenerStack.begin(), listenerStack.end(), &obj), listenerStack.end());
	};

	inline auto KeyboardCallback(GLFWwindow* win, int key, int scancode, int action, int mods) -> void {
		if (key == GLFW_KEY_F4 && action == GLFW_PRESS && mods == GLFW_MOD_ALT) {
			glfwSetWindowShouldClose(win, true);
		}
		else {
			if (keyMap.contains(key)) {
				if (action == GLFW_PRESS) {
					keyMap[key] = true;
				}
				else if (action == GLFW_RELEASE) {
					keyMap[key] = false;
				}
			}

			bool anyKeysDown = false;
			for (auto const& keyPair : keyMap) {
				if (keyPair.second) {
					anyKeysDown = true;
					break;
				}
			}
			keyboardActive = anyKeysDown;
				
		}
	}
};
#endif