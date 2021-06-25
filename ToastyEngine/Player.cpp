#include "Player.h"
#include "Timer.h"
#include "Keyboard.h"

#include <GLFW/glfw3.h>

void Player::handleInput() {
	if (Keyboard::keyMap[GLFW_KEY_LEFT_CONTROL]) {
		movementSpeed = 2*baseMovementSpeed;
	}
	else {
		movementSpeed = baseMovementSpeed;
	}

	float velocity = movementSpeed * Timer::getDeltaTime();
	if (Keyboard::keyMap[GLFW_KEY_W]) {
		transform.position += front * velocity;
	}
	if (Keyboard::keyMap[GLFW_KEY_S]) {
		transform.position -= front * velocity;
	}
	if (Keyboard::keyMap[GLFW_KEY_A]) {
		transform.position += right * velocity;
	}
	if (Keyboard::keyMap[GLFW_KEY_D]) {
		transform.position -= right * velocity;
	}
	if (Keyboard::keyMap[GLFW_KEY_SPACE]) {
		transform.position += up * velocity;
	}
	if (Keyboard::keyMap[GLFW_KEY_LEFT_SHIFT]) {
		transform.position -= up * velocity;
	}

	
}

void Player::update() {
	if (Keyboard::keyboardActive) {
		handleInput();
	}
	if (parent) {
		transform.position = parent->transform.position;
		parent->transform.rotation = transform.rotation;
	}
	updateVectors();
}
