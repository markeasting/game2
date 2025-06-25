#pragma once

#include "component/Camera.h"
#include "component/Component.h"

/**
 * @todo add settings struct
 */
class CameraController : public Component {
public:

    // Ref<Camera> m_camera = nullptr;
    
    float m_speed = 0.07; // 3.0f; (with dt)
    float m_fov = 70; // g_settings.fov;
    // float zoomspeed = 0.05f;

    /* @todo add 'mode' enum instead of booleans */
    bool m_enableFreeCam = true;
    bool m_autoRotate = false;
    float m_camRadius = 10.0f; // only used in auto-rotate mode -> move to other controller / component

    vec3 m_lookAtPos = vec3(0.0f, 0.0f, 0.0f);
    vec3 m_eulerRotation = vec3(-90.0f, 0.0f, 0.0f);

    // mat4 m_viewMatrix = mat4(1.0f);
    // mat4 m_projectionMatrix = mat4(1.0f);
    // mat4 m_viewProjectionMatrix = mat4(1.0f);

    CameraController() = default;

    ~CameraController();

    // @todo redirect to Camera methods
    // void setSize(float frameBufferWidth, float frameBufferHeight);
    // inline vec3 getForward() { return front; }
    // inline vec3 getUp() { return up; }
    // inline vec3 getRight() { return right; }
    // void bind() const;

    void update(float time, float dt) override;
};
