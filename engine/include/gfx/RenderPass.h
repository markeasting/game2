#pragma once

#include "common/ref.h"
#include "common/gl.h"
#include "gfx/FrameBuffer.h"
#include "gfx/Material.h"

struct RenderPassConfig {
    bool autoClear = true; // Automatically clear the framebuffer before rendering
};

class RenderPass {
public:

    /* @todo allow glEnable calls in a config, maybe add a 'callback' */
    RenderPassConfig m_config;

    RenderPass(Ref<Shader> shader, RenderPassConfig config = {}) 
        : m_material(shader), m_config(config) 
    {
        m_frameBuffer = FrameBuffer();
    }

    FrameBuffer m_frameBuffer;
    Material m_material;

    /**
     * @todo figure out which parts remain the same and can be abstracted out, 
     * e.g. same: binding the framebuffer, binding the material, etc.
     */
    virtual void bind(
        const FrameBuffer& readBuffer
        // GLuint drawBuffer
    ) {
        
        /* Bind the render pass framebuffer */
        // const auto& drawBuffer = m_frameBuffer.getId();
        // glBindFramebuffer(GL_DRAW_FRAMEBUFFER, drawBuffer);
        
        m_frameBuffer.bind();
        
        /* Set up OpenGL state */
        // m_setupFunc();
        // glReadBuffer(readBuffer);
        // glDrawBuffer(drawBuffer);

        if (m_config.autoClear) {
            glClear(GL_COLOR_BUFFER_BIT); // | GL_DEPTH_BUFFER_BIT
        }

        /* Bind the material / shader */
        m_material.bind();
        
        /* @todo Material::assignTexture() with an 'external' texture ID assigned */
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, readBuffer.getTexture());
    }

protected:
    // std::function<void()> m_setupFunc;
};

