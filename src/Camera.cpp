
#include "Camera.hpp"
#include <iostream>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/constants.hpp>

Camera::Camera() : position_({5.0, 5.0, 5.0f}), viewpoint_({0.0f, 0.0f, 0.0f}) {}

glm::mat4 Camera::getViewMatrix() const
{
    return glm::lookAt(position_, viewpoint_, {0.0f, 0.0f, 1.0f});
}

void Camera::handleInput(const Window& window)
{

    if (glfwGetKey(window.getHandle(), GLFW_KEY_W) == GLFW_PRESS)
    {
        if (glfwGetKey(window.getHandle(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            zoomIn();
        else
            moveForward();
    }

    if (glfwGetKey(window.getHandle(), GLFW_KEY_S) == GLFW_PRESS)
    {
        if (glfwGetKey(window.getHandle(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            zoomOut();
        else
            moveBackward();
    }

    if (glfwGetKey(window.getHandle(), GLFW_KEY_D) == GLFW_PRESS)
        moveRight();

    if (glfwGetKey(window.getHandle(), GLFW_KEY_A) == GLFW_PRESS)
        moveLeft();

    if (glfwGetKey(window.getHandle(), GLFW_KEY_Q) == GLFW_PRESS)
        rotateLeft();

    if (glfwGetKey(window.getHandle(), GLFW_KEY_E) == GLFW_PRESS)
        rotateRight();

    if (mouse_control_)
    {
        glm::vec2 cursor_position = window.getCursorPosition();

        float x_rotation = -2 * glm::pi<float>() / static_cast<float>(window.getWidth());
        float y_rotation = glm::pi<float>() / static_cast<float>(window.getHeight());

        glm::vec2 cursor_delta = cursor_position - mouse_origin_;  

        glm::vec3 view_vec3 = position_ - viewpoint_;

        // Do not allow camera to be vertical
        float vertical = glm::dot(glm::normalize(view_vec3), glm::vec3(0.0f, 0.0f, 1.0));
        if (vertical * std::copysign(1.0f, cursor_delta.y) > 0.99f) cursor_delta.y = 0;

        float yaw_angle = cursor_delta.x * x_rotation;
        float pitch_angle = cursor_delta.y * y_rotation;

        glm::mat4 yaw_rotation = glm::rotate(yaw_angle, glm::vec3(0.0f, 0.0f, 1.0f));
        glm::mat4 pitch_rotation = glm::rotate(pitch_angle, getRightVector());

        glm::vec4 view_vector = glm::vec4(view_vec3, 0.0f);

        position_ = glm::vec3(pitch_rotation * yaw_rotation * view_vector) + viewpoint_;
        setMouseOrigin(cursor_position);
    }
}

glm::vec3 Camera::getRightVector() const
{
    glm::vec3 view_vector = position_ - viewpoint_;
    return glm::cross(view_vector, glm::vec3(0.0f, 0.0f, 1.0f));
}

void Camera::moveForward()
{
    glm::vec3 step = stepsize_ * glm::vec3(glm::vec2(glm::normalize(viewpoint_ - position_)), 0.0f);
    position_ += step;
    viewpoint_ += step;
}

void Camera::moveBackward()
{
    glm::vec3 step = stepsize_ * glm::vec3(glm::vec2(glm::normalize(viewpoint_ - position_)), 0.0f);
    position_ -= step;
    viewpoint_ -= step;
}

void Camera::moveLeft()
{
    glm::vec2 diff = glm::vec2(glm::normalize(viewpoint_ - position_));
    glm::vec3 step = stepsize_ * glm::vec3(diff.y, -diff.x, 0.0f);
    position_ -= step;
    viewpoint_ -= step;
}

void Camera::moveRight()
{
    glm::vec2 diff = glm::vec2(glm::normalize(viewpoint_ - position_));
    glm::vec3 step = stepsize_ * glm::vec3(diff.y, -diff.x, 0.0f);
    position_ += step;
    viewpoint_ += step;
}

void Camera::rotateRight()
{
    glm::vec2 diff = glm::vec2(glm::normalize(viewpoint_ - position_));
    glm::vec3 step = stepsize_ * glm::vec3(diff.y, -diff.x, 0.0f);
    position_ += step;
}

void Camera::rotateLeft()
{
    glm::vec2 diff = glm::vec2(glm::normalize(viewpoint_ - position_));
    glm::vec3 step = stepsize_ * glm::vec3(diff.y, -diff.x, 0.0f);
    position_ -= step;
}

void Camera::zoomIn()
{
    zoom(stepsize_);
}

void Camera::zoomOut()
{
    zoom(-stepsize_);
}

void Camera::zoom(float step)
{
    glm::vec3 difference = viewpoint_ - position_;
    float norm = glm::length(difference);

    step = std::clamp(step, -MAX_ZOOM, MAX_ZOOM);

    if (step > 0.0f) // Limit closest approach
        if (step > (norm - 10.0f))
            step = norm - 10.0f;

    glm::vec3 movement = step * difference / norm;
    position_ += movement;
}

void Camera::setViewpoint(glm::vec3 viewpoint) { viewpoint_ = viewpoint; }
void Camera::setPosition(glm::vec3 position) { position_ = position; }

