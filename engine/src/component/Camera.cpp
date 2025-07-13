
#include "component/Camera.h"
#include "component/Transform.h"
#include "gameobject/GameObject.h"

#include <SDL2/SDL.h>

Camera::Camera() {}
Camera::Camera(CameraSettings settings) : m_settings(settings) {}

Camera::~Camera() {}

void Camera::setSize(
    const int frameBufferWidth, 
    const int frameBufferHeight
) {

    m_projectionMatrix = glm::perspective(
        glm::radians(m_settings.fov), 
        (float) frameBufferWidth / (float) frameBufferHeight, 
        m_settings.nearPlane,
        m_settings.farPlane
    );
    
    // @todo only invalidate/update if size has changed
    m_frameBuffer.setSize(frameBufferWidth, frameBufferHeight);

}

void Camera::bind() const {
    m_frameBuffer.bind();
}

void Camera::update(float time, float dt) {

    auto transform = gameObject->getComponent<Transform>();

    // front = glm::normalize(_front); // set by camera controller
    right = glm::normalize(glm::cross(front, vec3(0.0f, 1.0f, 0.0f)));
    up    = glm::normalize(glm::cross(right, front));

    const auto&& pos = transform->getPosition();
    
    // m_viewMatrix = glm::lookAt(m_position, m_position + front, vec3(0.0, 1.0f, 0.0));
    m_viewMatrix = glm::lookAt(pos, pos + front, up);
    
    m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
}
