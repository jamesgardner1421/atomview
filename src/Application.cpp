
#define GLFW_INCLUDE_VULKAN
#define VK_ENABLE_BETA_EXTENSIONS
#include <GLFW/glfw3.h>

#include "Application.hpp"

Application::Application(int width, int height)
    :
    window_(width, height),
    camera_(),
    renderer_(window_, camera_)
{
    glfwSetWindowUserPointer(window_.getHandle(), this);
}

void Application::runMainLoop() {

    while (!window_.shouldClose()) {

        pollEvents();

        beginImGuiFrame();

        camera_.handleInput(window_);

        ImGui::Begin("Hello, world!");
        ImGui::Text("This is some useful text.");
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();

        endImGuiFrame();

        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();

        renderer_.drawFrame(
            atomic_system_->getPositions(),
            atomic_system_->getColors(),
            atomic_system_->getSelectedAtoms()
        );
    }

}

void Application::setAtomicSystem(std::unique_ptr<AtomicSystem> system)
{
    atomic_system_ = std::move(system);
    camera_.setViewpoint(atomic_system_->getCentre());
    camera_.setPosition(atomic_system_->getInitialCameraPosition());
}

void Application::beginImGuiFrame()
{
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void Application::endImGuiFrame()
{
    ImGui::Render();
}

