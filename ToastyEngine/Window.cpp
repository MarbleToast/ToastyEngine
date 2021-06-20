#include "Window.h"
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Diagnostics.h"

Window::Window(int w, int h, const std::string_view name, bool fullscreen): clientWidth(w), clientHeight(h) {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_STENCIL_BITS, 8);
    glfwWindowHint(GLFW_DEPTH_BITS, 24);
    glfwWindowHint(GLFW_SAMPLES, 16);
    
    if (!(m_glfwWindow = glfwCreateWindow(w, h, name.data(), fullscreen ? glfwGetPrimaryMonitor() : NULL, NULL))) {
        Diagnostics::Throw("Window failed to initialise (code {}).", glfwGetError(NULL));
    }

    glfwMakeContextCurrent(m_glfwWindow);
    
    if (!gladLoadGL(glfwGetProcAddress)) {
        Diagnostics::Throw("GLAD failed to initialise (code {}).", glfwGetError(NULL));
        exit(-1);
    }

    
    glfwSetWindowUserPointer(m_glfwWindow, this);
    glfwSetInputMode(m_glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetFramebufferSizeCallback(m_glfwWindow, WindowResizeCallback);
}

auto Window::ViewportDidResize(int w, int h) -> void {
    clientHeight = h;
    clientWidth = w;
    glViewport(0, 0, w, h);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

Window::~Window(){
    glfwDestroyWindow(m_glfwWindow);
}
