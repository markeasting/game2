#pragma once

#include "common/glm.h"

#include "component/Component.h"
#include "gfx/FrameBuffer.h"

struct CameraSettings {
    float fov = 70.0f;          // Field of view in degrees
    float nearPlane = 0.01f;    // Near clipping plane
    float farPlane = 500.0f;    // Far clipping plane
};

class Camera : public Component {
public:

    CameraSettings m_settings;

    // @todo pass matrices with a uniform buffer object
    mat4 m_viewMatrix = mat4(1.0f);
    mat4 m_projectionMatrix = mat4(1.0f);
    mat4 m_viewProjectionMatrix = mat4(1.0f);

    FrameBuffer m_frameBuffer;

    Camera();
    Camera(CameraSettings settings);
    ~Camera();

    void setSize(
        const int frameBufferWidth, 
        const int frameBufferHeight
    );

    inline vec3 getForward() { return front; }
    inline vec3 getUp() { return up; }
    inline vec3 getRight() { return right; }

    void bind() const;
    void update(float time, float dt) override;

protected:
    friend class CameraController;

    vec3 up = vec3(0.0f, 1.0f, 0.0f);
    vec3 front = vec3(0.0f, 0.0f, -1.0f);
    vec3 right = vec3(1.0f, 0.0f, 0.0f);

};
