
#include "gameobject/GameObject.h"

#include "component/Camera.h"
#include "component/Transform.h"
#include "component/CameraController.h"

#include <SDL2/SDL.h>

// CameraController::CameraController(Ref<Camera> camera) : m_camera(camera) {}

CameraController::~CameraController() {}

void CameraController::update(float time, float dt) {

    // SDL_SetWindowGrab();
    SDL_SetRelativeMouseMode(m_autoRotate 
        ? SDL_FALSE 
        : m_enableFreeCam ? SDL_TRUE : SDL_FALSE
    );
    
    auto transform  = gameObject->getComponent<Transform>();
    auto camera     = gameObject->getComponent<Camera>();

    /* @todo add 'mode' enum instead of booleans */
    if (m_autoRotate) {

        // m_camRadius = glm::distance(m_position, m_lookAtPos);

        transform->m_position.x = sin(time) * m_camRadius;
        transform->m_position.z = cos(time) * m_camRadius;
        transform->m_position.y = 1.0f;

        // m_camera->setPosition(m_camera->m_position);
        // m_camera->m_lookAtPos = m_position + m_camera->front;
        // m_camera->m_lookAtPos = m_lookAtPos;

        camera->front = glm::normalize(m_lookAtPos - transform->m_position);

    } else if (m_enableFreeCam) {

        float posDelta = m_speed; // * time.dt;

        const Uint8* state = SDL_GetKeyboardState(NULL);

        if (state[SDL_SCANCODE_LSHIFT])
            posDelta *= 5.0f;

        if (state[SDL_SCANCODE_LCTRL])
            posDelta *= 0.1f;

        if (state[SDL_SCANCODE_W])
            transform->m_position += camera->front * posDelta;
        if (state[SDL_SCANCODE_S])
            transform->m_position -= camera->front * posDelta;
        if (state[SDL_SCANCODE_A])
            transform->m_position -= camera->right * posDelta;
        if (state[SDL_SCANCODE_D])
            transform->m_position += camera->right * posDelta;

        int x, y;
        SDL_GetRelativeMouseState(&x, &y);

        m_eulerRotation.x += x * 0.5f;
        m_eulerRotation.y -= y * 0.5f;

        m_eulerRotation.y = std::clamp(m_eulerRotation.y, -89.5f, 89.5f);

        vec3 _front;
        _front.x = cos(glm::radians(m_eulerRotation.x)) * cos(glm::radians(m_eulerRotation.y));
        _front.y = sin(glm::radians(m_eulerRotation.y));
        _front.z = sin(glm::radians(m_eulerRotation.x)) * cos(glm::radians(m_eulerRotation.y));

        // m_camera->m_position = m_camera->m_position;

        camera->front = glm::normalize(_front);
        // m_camera->m_lookAtPos = m_position + m_camera->front;
    }

    // m_camera->updateMatrix();
}
