
#include "Window.hpp"
#include "Application.hpp"
#include <iostream>

static void framebufferResizeCallback(GLFWwindow* windowHandle, int width, int height) {
    auto app = reinterpret_cast<Application*>(glfwGetWindowUserPointer(windowHandle));
    app->getWindow().framebufferResized = true;
}

static void mouseButtonCallback(GLFWwindow* windowHandle, int button, int action, int mods)
{
    auto app = reinterpret_cast<Application*>(glfwGetWindowUserPointer(windowHandle));
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        auto& camera = app->getCamera();
        camera.toggleMouseControl();
        camera.setMouseOrigin(app->getWindow().getCursorPosition());
    }

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
    {
        app->getCamera().toggleMouseControl();
    }

}

static void scrollCallback(GLFWwindow* windowHandle, double xoffset, double yoffset)
{
    auto app = reinterpret_cast<Application*>(glfwGetWindowUserPointer(windowHandle));
    app->getCamera().zoom(yoffset);
}

Window::Window(int width, int height)
    : width_(width), height_(height)
{
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    window_ = glfwCreateWindow(width, height, "atomview", nullptr, nullptr);
    glfwSetFramebufferSizeCallback(window_, framebufferResizeCallback);

    if (glfwRawMouseMotionSupported())
        glfwSetInputMode(window_, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    glfwSetMouseButtonCallback(window_, mouseButtonCallback);

    glfwSetScrollCallback(window_, scrollCallback);
}

Window::~Window()
{
    glfwDestroyWindow(window_);
    glfwTerminate();
}

void Window::updateFramebufferSize()
{
    glfwGetFramebufferSize(window_, &width_, &height_);
}

glm::vec2 Window::getCursorPosition() const
{
    double xpos, ypos;
    glfwGetCursorPos(window_, &xpos, &ypos);
    return { xpos, ypos };
}

