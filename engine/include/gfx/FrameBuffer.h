#pragma once

#include "common/gl.h"
#include "gfx/Texture.h"

/**
 * https://learnopengl.com/Advanced-OpenGL/Framebuffers 
 */
class FrameBuffer {
public:

    /**
     * @brief Default constructor.
     * glGenFramebuffers
     */
    FrameBuffer();

    void create(
        const int width, 
        const int height
    );

    /**
     * @brief Invalidate the framebuffer and delete GPU resources.
     */
    void invalidate() const;

    void bind() const;

    GLuint getTexture() const;

protected:
	GLuint m_fbo = 0;
	GLuint m_rbo = 0;

    GLuint m_textureColorbuffer = 0;

    Texture m_texture;
};
