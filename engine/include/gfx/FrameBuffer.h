#pragma once

#include "common/gl.h"
#include "gfx/Texture.h"

/**
 * Frame Buffer Object (fbo) for off-screen rendering.
 * https://learnopengl.com/Advanced-OpenGL/Framebuffers 
 */
class FrameBuffer {
public:

    /**
     * @brief Default constructor.
     * glGenFramebuffers
     */
    FrameBuffer();

    /**
     * @brief Creates the framebuffer. 
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

    void bind() const;

    /** @return fbo handle (Frame Buffer Object) */
    GLuint getId() const { return m_fbo; }

    GLuint getTexture() const { return m_texture.getId(); }

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
