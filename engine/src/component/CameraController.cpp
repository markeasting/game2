
#include "gameobject/GameObject.h"

#include "component/Camera.h"
#include "component/Transform.h"
#include "component/CameraController.h"

#include <SDL2/SDL.h>

void CameraController::update(float time, float dt) {

    // SDL_SetWindowGrab();
    SDL_SetRelativeMouseMode(m_autoRotate 
        ? SDL_FALSE 
        : m_enableFreeCam ? SDL_TRUE : SDL_FALSE
    );
    
    auto transform  = gameObject->getComponent<Transform>();
    auto camera     = gameObject->getComponent<Camera>();

    vec3 pos = transform->getPosition();

    /* @todo add 'mode' enum instead of booleans */
    if (m_autoRotate) {

        // m_camRadius = glm::distance(m_position, m_lookAtPos);

        pos.x = sin(time) * m_camRadius;
        pos.z = cos(time) * m_camRadius;
        pos.y = 1.0f;
        
        transform->setPosition(pos);

        camera->front = glm::normalize(m_lookAtPos - pos);

        return;
    }
    
    if (m_enableFreeCam) {

        float posDelta = m_speed; // * time.dt;

        const Uint8* state = SDL_GetKeyboardState(NULL);

        if (state[SDL_SCANCODE_LSHIFT])
            posDelta *= 5.0f;

        if (state[SDL_SCANCODE_LCTRL])
            posDelta *= 0.1f;
            
        if (state[SDL_SCANCODE_W])
            pos += camera->front * posDelta;
        if (state[SDL_SCANCODE_S])
            pos -= camera->front * posDelta;
        if (state[SDL_SCANCODE_A])
            pos -= camera->right * posDelta;
        if (state[SDL_SCANCODE_D])
            pos += camera->right * posDelta;

        transform->setPosition(pos);

        int x, y;
        SDL_GetRelativeMouseState(&x, &y);

        m_eulerRotation.x += x * 0.5f;
        m_eulerRotation.y -= y * 0.5f;

        m_eulerRotation.y = std::clamp(m_eulerRotation.y, -89.5f, 89.5f);

        vec3 _front;
        _front.x = cos(glm::radians(m_eulerRotation.x)) * cos(glm::radians(m_eulerRotation.y));
        _front.y = sin(glm::radians(m_eulerRotation.y));
        _front.z = sin(glm::radians(m_eulerRotation.x)) * cos(glm::radians(m_eulerRotation.y));

        camera->front = glm::normalize(_front);
    } else {
        camera->front = glm::normalize(m_lookAtPos - pos);
        camera->right = glm::normalize(glm::cross(camera->front, vec3(0.0f, 1.0f, 0.0f)));
        camera->up = glm::normalize(glm::cross(camera->right, camera->front));
    }

    // m_camera->updateMatrix();
}
