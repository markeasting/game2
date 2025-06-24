#pragma once

#include "obj/Object3D.h"
#include "gfx/FrameBuffer.h"

class Camera : public Object3D {
public:
    
    float m_near = 0.01f; // g_settings.cameraNear;
    float m_far = 500.0f; // g_settings.cameraFar;
    float m_fov = 70; // g_settings.fov;

    // @todo pass with a uniform buffer object
    mat4 m_viewMatrix = mat4(1.0f);
    mat4 m_projectionMatrix = mat4(1.0f);
    mat4 m_viewProjectionMatrix = mat4(1.0f);

    FrameBuffer m_frameBuffer;

    Camera();
    ~Camera();

    void setSize(
        const int frameBufferWidth, 
        const int frameBufferHeight
    );

    // vec3 getOrientation(); // @TODO use in audio listener setOrientation()
    inline vec3 getForward() { return front; }
    inline vec3 getUp() { return up; }
    inline vec3 getRight() { return right; }

    void bind() const;
    void update();

protected:
    friend class CameraController;

    vec3 up = vec3(0.0f, 1.0f, 0.0f);
    vec3 front = vec3(0.0f, 0.0f, -1.0f);
    vec3 right = vec3(1.0f, 0.0f, 0.0f);

};
