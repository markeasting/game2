#pragma once

#include "common/ref.h"
#include "common/gl.h"
#include "common/glm.h"
#include "gfx/FrameBuffer.h"
#include "gfx/Material.h"

/** 
 * Render pass configuration.
 * 
 * @todo allow glEnable calls in a config, maybe add a 'callback'
 */
struct RenderPassConfig {
    /** Automatically clear the framebuffer before rendering */
    bool autoClear = true;

    /** Clear color for the framebuffer */
    vec4 clearColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
};

class RenderPass {
public:

    RenderPassConfig m_config;

    /** Framebuffer for the render pass */
    FrameBuffer m_frameBuffer;

    /** Material applied to the full-screen quad */
    Material m_material;

    RenderPass(Material material, RenderPassConfig config = {}) 
        : m_material(material), m_config(config) 
    {
        m_frameBuffer = FrameBuffer();
    }

    /**
     * @todo figure out which parts remain the same and can be abstracted out, 
     * e.g. same: binding the draw framebuffer, binding the material, etc.
     */
    virtual void bind(
        const FrameBuffer& readBuffer
    ) {
        
        /* Bind the render pass draw framebuffer */
        // m_frameBuffer.bind();
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_frameBuffer.getId());
        
        /* Set up OpenGL state */
        // m_setupFunc();
        // glReadBuffer(readBuffer);
        // glDrawBuffer(drawBuffer);

        if (m_config.autoClear) {
            glClearColor(
                m_config.clearColor.r,
                m_config.clearColor.g,
                m_config.clearColor.b,
                m_config.clearColor.a
            );
            glClear(GL_COLOR_BUFFER_BIT); // | GL_DEPTH_BUFFER_BIT // @todo add depth buffer attachment in FrameBuffer object
        }

        /* Bind the material / shader */
        m_material.bind();
        
        /* @todo Material::assignTexture() with an 'external' texture ID assigned */
        m_material.setUniform("u_readBuffer", 0); // Texture unit 0 for the read buffer - same as glBindTexture?
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, readBuffer.getTexture().getId());
    }

    /**
	 * @brief Updates the size of the render pass framebuffer
	 */
	void setSize(
		GLint x,
		GLint y,
		GLsizei width,
		GLsizei height
	) {
        m_frameBuffer.create(width, height);
    };

protected:
    // std::function<void()> m_setupFunc;
};

