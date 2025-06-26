#pragma once

#include "component/Camera.h"
#include "component/Component.h"

/**
 * @todo add settings struct
 */
class CameraController : public Component {
public:

    float m_speed = 0.07; // 3.0f; (with dt)
    float m_fov = 70; // g_settings.fov;
    // float zoomspeed = 0.05f;

    /* @todo add 'mode' enum instead of booleans */
    bool m_enableFreeCam = true;
    bool m_autoRotate = false;
    float m_camRadius = 10.0f;

    vec3 m_lookAtPos = vec3(0.0f, 0.0f, 0.0f);
    vec3 m_eulerRotation = vec3(-90.0f, 0.0f, 0.0f);

    CameraController() = default;
    ~CameraController() = default;

    void update(float time, float dt) override;
};
