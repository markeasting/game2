
#include "core/Camera.h"

#include <SDL_keyboard.h>
#include <SDL_mouse.h>
#include <iostream>
// #include "input/Keyboard.h"

Camera::Camera() {}

Camera::~Camera() {}

void Camera::setSize(
    const int frameBufferWidth, 
    const int frameBufferHeight
) {

    m_projectionMatrix = glm::perspective(
        glm::radians(m_fov), 
        (float) frameBufferWidth / (float) frameBufferHeight, 
        m_near,
        m_far
    );
    
    // @todo only invalidate/update if size has changed
    m_frameBuffer.create(frameBufferWidth, frameBufferHeight);

}

void Camera::bind() const {
    m_frameBuffer.bind();
}

void Camera::update() {

    // front = glm::normalize(_front); // set by camera controller
    right = glm::normalize(glm::cross(front, vec3(0.0f, 1.0f, 0.0f)));
    up    = glm::normalize(glm::cross(right, front));
    
    // m_viewMatrix = glm::lookAt(m_position, m_position + front, vec3(0.0, 1.0f, 0.0));
    m_viewMatrix = glm::lookAt(m_position, m_position + front, up);
    
    m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
}
