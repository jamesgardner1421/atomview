#include <iostream>
#include <stdexcept>
#include <filesystem>

#include "Application.hpp"

int main(int argc, char *argv[]) {
    std::cout << "Launching atomview" << std::endl;
    std::cout << std::filesystem::current_path() << std::endl;

    if (argc != 2)
    {
        std::cerr << "You must provide the geometry filename as a command line argument." << std::endl;
        return EXIT_FAILURE;
    }

    std::string filename{argv[1]};
    std::cout << "Filename:" << filename << std::endl;

    Application app{800, 600};

    auto system = std::make_unique<AtomicSystem>(filename);
    app.setAtomicSystem(std::move(system));

    /* glfwSetMouseButtonCallback(renderer.window, mouse_button_callback); */
    /* if (glfwRawMouseMotionSupported()) */
    /*     glfwSetInputMode(renderer.window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE); */

    try {
        app.runMainLoop();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

