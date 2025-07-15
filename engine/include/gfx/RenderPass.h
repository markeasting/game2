#pragma once

#include "common/ref.h"
#include "common/gl.h"
#include "common/glm.h"
#include "component/Mesh.h"
#include "gfx/FrameBuffer.h"
#include "gfx/Material.h"

/** 
 * Render pass configuration.
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
    Ref<Material> m_material = nullptr;

    RenderPass(
        Material material, 
        RenderPassConfig config = {}, 
        FrameBufferSettings frameBufferSettings = {
            .attachDefaultColorAttachment = true,   // Attach GL_COLOR_ATTACHMENT0 automatically
            .attachRenderBufferObject = false       // No RBO is required for render passes
        }
    );

    /**
	 * @brief Updates the size of the render pass framebuffer
	 */
	virtual void setSize(
		GLint x,
		GLint y,
		GLsizei width,
		GLsizei height
	);

    /**
     * @brief Binds the render pass framebuffer and sets up OpenGL state.
     * @param cameraBuffer Camera buffer (initial render pass).
     * @param prevPassBuffer Previous render pass buffer. 
     */
    virtual void bind(
        const FrameBuffer& cameraBuffer,
        const FrameBuffer& prevPassBuffer
    );

    /** 
     * @brief Draws the full-screen quad with the render pass material.
     * @param fullscreenQuad The full-screen quad to draw.
     * @note This method could be overridden in derived classes to implement specific rendering logic.
     */
    virtual void draw(Mesh& fullscreenQuad);
    

protected:
    // std::function<void()> m_setupFunc;
    
    /**
     * @brief (Internal) Draws a full-screen quad with the given material.
     * @param fullscreenQuad The full-screen quad to draw.
     * @param material The material to use for rendering.
     */
    void _drawQuad(
        Mesh& fullscreenQuad,
        Ref<Material> material
    );
};

