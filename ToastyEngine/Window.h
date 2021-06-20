#pragma once
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <string_view>

class Window {
public:
    int clientWidth, clientHeight;
    Window(int w, int h, const std::string_view name, bool fullscreen);
    ~Window();
    auto ViewportDidResize(int w, int h) -> void;

    constexpr auto getWindow() const->GLFWwindow* {
        return m_glfwWindow;
    };

private:
    GLFWwindow* m_glfwWindow;

    static inline auto WindowResizeCallback(GLFWwindow* win, int w, int h) -> void {
        Window* window = static_cast<Window*>(glfwGetWindowUserPointer(win));
        window->ViewportDidResize(w, h);
    }

};