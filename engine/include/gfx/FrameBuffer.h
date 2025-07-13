#pragma once

#include "common/gl.h"
#include "common/ref.h"
#include "gfx/Texture.h"
#include <unordered_map>

struct FrameBufferSettings {
    /** Attach GL_COLOR_ATTACHMENT0 automatically */
    bool attachDefaultColorAttachment = true;
    
    /** Attach a render buffer object for depth and stencil / GL_DEPTH24_STENCIL8 */
    bool attachRenderBufferObject = false;

    /** Internal format of the color attachments - e.g. GL_RGBA or GL_RGBA16F for HDR */
    // GLenum internalFormat = GL_RGBA; 
};

/**
 * Frame Buffer Object (fbo) for off-screen rendering.
 * https://learnopengl.com/Advanced-OpenGL/Framebuffers 
 */
class FrameBuffer {
public:

    /**
     * @brief Default constructor.
     * @see glGenFramebuffers()
     */
    FrameBuffer(FrameBufferSettings settings = {});

    /**
     * @brief Invalidates the current framebuffer and updates the size. 
     * @param width Width of the framebuffer.
     * @param height Height of the framebuffer.
     */
    virtual void setSize(
        const int width, 
        const int height
    );

    /**
     * @brief Invalidate the framebuffer and delete GPU resources.
     */
    virtual void invalidate();

    /** 
     * @brief Binds the framebuffer for rendering.
     * @see glBindFramebuffer()
     */
    virtual void bind() const;

    /** @return fbo handle (Frame Buffer Object) */
    GLuint getId() const { return m_fbo; }

    /** Shortcut for getting the first color attachment / texture */
    const Ref<Texture> getTexture() const { 
        return getColorAttachment(GL_COLOR_ATTACHMENT0);
    }

    /**
     * @brief Get a color attachment by its attachment point.
     * @param attachment Attachment point, e.g. GL_COLOR_ATTACHMENT0.
     * @return Texture attached to the framebuffer at the given attachment point.
     * @throws std::out_of_range if the attachment is out of range (greater than GL_COLOR_ATTACHMENT15).
     */
    const Ref<Texture> getColorAttachment(GLenum attachment) const {

        // printf("[FrameBuffer] Getting color attachment %d\n", attachment);

        // Slow
        // GLint maxColorAttachments;
        // glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &maxColorAttachments);
        
        // GLenum attach = GL_COLOR_ATTACHMENT0 + attachment;

        if (attachment > GL_COLOR_ATTACHMENT15) {
            printf("[FrameBuffer] Error: Attachment %d is out of range (max is 15)\n", attachment);
            throw std::out_of_range("[FrameBuffer] ERR");
        }

        // if (m_colorAttachments.count(attachment)) {
        //     return m_colorAttachments.at(attachment);
        // }
        // return nullptr;

        return m_colorAttachments.at(attachment);
    }

    int getWidth() const { return m_width; }
    int getHeight() const { return m_height; }

    /**
     * @brief Adds a color attachment to the framebuffer.
     * @param attachment Attachment point, e.g. GL_COLOR_ATTACHMENT0.
     * @param bind Whether to bind/unbind the framebuffer when adding the attachment.
     */
    void addColorAttachment(
        GLenum attachment,
        bool bind = true
    );

protected:

    FrameBufferSettings m_settings;

    int m_width = 100;
    int m_height = 100;

    /** 
     * @brief Color attachments of the framebuffer.
     * [attachment (GL_COLOR_ATTACHMENTX), Texture]
     */
    std::unordered_map<GLenum, Ref<Texture>> m_colorAttachments;

    /** Reference to the OpenGL FrameBuffer ID */
	GLuint m_fbo = 0;

    /** Reference to the OpenGL RenderBuffer ID */
	GLuint m_rbo = 0;

    // GLuint m_textureColorbuffer = 0;
    /** Texture (color attachment) of the framebuffer */
    // Texture m_texture;
};
