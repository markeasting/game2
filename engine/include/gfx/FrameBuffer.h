#pragma once

#include "common/gl.h"
#include "gfx/Texture.h"

/**
 * Frame Buffer Object (fbo) for off-screen rendering.
 * https://learnopengl.com/Advanced-OpenGL/Framebuffers 
 * @todo add ability to change attachments, e.g. depth buffer, stencil buffer, etc.
 */
class FrameBuffer {
public:

    /**
     * @brief Default constructor.
     * glGenFramebuffers
     */
    FrameBuffer();

    /**
     * @brief Invalidates the current framebuffer and creates a new framebuffer. 
     * @param width Width of the framebuffer.
     * @param height Height of the framebuffer.
     */
    void create(
        const int width, 
        const int height
    );

    /**
     * @brief Invalidate the framebuffer and delete GPU resources.
     */
    void invalidate();

    /** 
     * @brief Binds the framebuffer for rendering.
     * @see glBindFramebuffer()
     */
    void bind() const;

    /** @return fbo handle (Frame Buffer Object) */
    GLuint getId() const { return m_fbo; }

    Texture getTexture() const { return m_texture; }

    int getWidth() const { return m_texture.getWidth(); }
    int getHeight() const { return m_texture.getHeight(); }

protected:

    /** Reference to the OpenGL FrameBuffer ID */
	GLuint m_fbo = 0;

    /** Reference to the OpenGL RenderBuffer ID */
	GLuint m_rbo = 0;

    // GLuint m_textureColorbuffer = 0;
    Texture m_texture;
};
