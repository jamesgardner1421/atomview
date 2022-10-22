#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Window.hpp"

class Camera
{
public:

    Camera();

    glm::mat4 getViewMatrix() const;

    void handleInput(const Window& window);

    void moveForward();
    void moveBackward();
    void moveLeft();
    void moveRight();
    void rotateRight();
    void rotateLeft();
    void zoomIn();
    void zoomOut();
    void zoom(float step);
    void setViewpoint(glm::vec3 viewpoint);
    void setPosition(glm::vec3 position);

    void toggleMouseControl() { mouse_control_ = !mouse_control_; }
    void setMouseOrigin(glm::vec2 cursor) { mouse_origin_ = cursor; }
    glm::vec2 getMouseOrigin() const { return mouse_origin_ ; }

private:

    glm::vec3 getRightVector() const;

    glm::vec3 position_;
    glm::vec3 viewpoint_;

    glm::vec3 up_ = {0.0f, 0.0f, 1.0f};

    bool mouse_control_{false};
    glm::vec2 mouse_origin_{0.0, 0.0};

    static constexpr float stepsize_ = 0.1f;
    static constexpr float anglestepsize_ = 0.01f;
    static constexpr float MAX_ZOOM = 25.0f;
};

