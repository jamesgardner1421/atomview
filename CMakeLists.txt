cmake_minimum_required(VERSION 3.24)

project(
    atomview
    VERSION 0.1.0
    LANGUAGES CXX
    )

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

add_executable(${PROJECT_NAME} src/main.cpp src/Application.cpp src/Window.cpp
    src/Icosphere.cpp src/Renderer.cpp src/AtomicSystem.cpp src/Camera.cpp)

find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(imgui REQUIRED)

add_subdirectory(external/chemfiles)

target_include_directories(${PROJECT_NAME}
    PRIVATE external/chemfiles/include
    PRIVATE ${PROJECT_BINARY_DIR}/external/chemfiles/include
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Vulkan::Vulkan
    glfw
    glm::glm
    chemfiles
    imgui::imgui
)

