#pragma once

#include "AtomicSystem.hpp"
#include "Renderer.hpp"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>

class Application
{
public:
    Application(int width, int height);
    void runMainLoop();

    void setAtomicSystem(std::unique_ptr<AtomicSystem> system);

    Window& getWindow() { return window_; }
    Camera& getCamera() { return camera_; }

private:

    void pollEvents() { glfwPollEvents(); }
    void beginImGuiFrame();
    void endImGuiFrame();

    Window window_;
    Camera camera_;
    Renderer renderer_;

    std::unique_ptr<AtomicSystem> atomic_system_;
};

