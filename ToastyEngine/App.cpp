#include "App.h"
#include "Shader.h"
#include "Camera.h"
#include "Player.h"
#include "Texture.h"
#include "Timer.h"
#include "Keyboard.h"
#include "Model.h"
#include "Renderer.h"

#include <filesystem>
#include <iostream>
#include <vector>

void Application::initShaders() {
    ResourceCache::loadShader("standardShader", "standard.vertex.glsl", "standard.fragment.glsl", "");
}

void Application::init() {
    initShaders();
}

void Application::run() {
    init();

	GLFWwindow* main = window->getWindow();

    glEnable(GL_DEPTH_TEST);
    glCullFace(GL_BACK);

    Model backpack("assets/backpack/backpack.obj");

    Renderer renderer;
    Shader standardShader = *ResourceCache::getShader("standardShader");

    Player player({5, 0, 0});
    Camera mainCamera({1, 0, 0});

    Keyboard::addListener(player);
    mainCamera.setParent(player);

	while (!glfwWindowShouldClose(main)) {
        float deltaTime = Timer::getDeltaTime();
        Camera* currentCamera = Camera::getCurrentCamera();

        glClearColor(0.3f, 0.1f, 0.1f, 0.1f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        standardShader.activate();

        if (Keyboard::keyboardActive) {
            player.handleInput();
        }

        currentCamera->update();
        player.update();
        
        Mat4 projection = glm::perspective(glm::radians(currentCamera->fOV), (float)window->clientWidth / (float)window->clientHeight, 0.1f, 100.0f);
        standardShader.setMat4("projection", projection);

        // camera/view transformation
        Mat4 view = currentCamera->getViewMatrix();
        standardShader.setMat4("view", view);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));	// it's a bit too big for our scene, so scale it down
        standardShader.setMat4("model", model);
        backpack.Draw(standardShader);

		glfwSwapBuffers(main);
		glfwPollEvents();
	}
}
