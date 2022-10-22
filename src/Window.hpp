#pragma once

#define GLFW_INCLUDE_VULKAN
#define VK_ENABLE_BETA_EXTENSIONS
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

class Window
{
public:
    Window(int width, int height);
    ~Window();

    void updateFramebufferSize();
    int getWidth() const { return width_; }
    int getHeight() const { return height_; }
    bool shouldClose() const { return glfwWindowShouldClose(window_); }
    GLFWwindow* getHandle() const { return window_; }

    glm::vec2 getCursorPosition() const;

    bool framebufferResized{false};

private:
    GLFWwindow* window_;
    int width_, height_;
};

