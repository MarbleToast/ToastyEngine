#include "App.h"
#include "Shader.h"
#include "Camera.h"
#include "Player.h"
#include "Texture.h"
#include "Timer.h"
#include "Keyboard.h"
#include "Model.h"
#include "Renderer.h"
#include "RandomUtils.h"

#include <filesystem>
#include <iostream>
#include <vector>

void Application::initShaders() {
    ResourceCache::loadShader("standardShader", "standard.vertex.glsl", "standard.fragment.glsl", "");
}

void Application::init() {    

    glEnable(GL_DEPTH_TEST);
    glCullFace(GL_BACK);

    initShaders();
}

void Application::run() {
    init();

	GLFWwindow* main = window->getWindow();

    Renderer renderer;

    ModelPtr duck = ResourceCache::loadModel("assets/ducky/ducky.obj");

    std::srand(glfwGetTime());
    
    for (float i = 0; i < 100;) {
        renderer.AddModel(duck, "standardShader", {
            {
                getRandomDoubleRange(500, -500),
                getRandomDoubleRange(500, -500),
                getRandomDoubleRange(500, -500),
            },
            { 
                getRandomDouble(2),
                getRandomDouble(2)
            },
            {
                .1,
                .1,
                .1
            }
        });

        ++i;
    }

    // Creation of camera object here
    Player player({5, 0, 0});
    Camera mainCamera({1, 0, 0}, window);

    mainCamera.setParent(player);

	while (!glfwWindowShouldClose(main)) {
        float deltaTime = Timer::getDeltaTime();
        Camera* currentCamera = Camera::getCurrentCamera();

        glClearColor(0.01f, 0.01f, 0.01f, 0.01f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        currentCamera->update();
        player.update();
        
        renderer.Draw(window, currentCamera);

		glfwSwapBuffers(main);
		glfwPollEvents();
	}

    glDisable(GL_DEPTH_TEST);
    renderer.release();
}
