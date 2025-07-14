#pragma once

#include "common/gl.h"
#include "common/ref.h"
#include <unordered_map>
#include <vector>

struct FrameBufferSettings {

    /** 
     * Whether to use a gbuffer for deferred rendering 
     * @todo maybe add GBuffer class that extends FrameBuffer 
     */
    // bool asGbuffer = false;

    /** Whether to use HDR (floating point) textures, e.g. GL_RGBA16F */
    bool useHdr = true;

    /** Attach GL_COLOR_ATTACHMENT0 automatically */
    bool attachDefaultColorAttachment = false;
    
    /** Attach a render buffer object for depth and stencil / GL_DEPTH24_STENCIL8 */
    bool attachRenderBufferObject = false;
};

struct FboAttachment {
    GLenum attachment;  // e.g. GL_COLOR_ATTACHMENT0
    GLenum target;      // e.g. GL_TEXTURE_2D
    GLuint texture;     // Texture object ID
    GLint level = 0;    // Mip level, default 0
    GLint internalformat = GL_RGBA16F;  // Format of the texture, e.g. GL_RGBA16F or GL_RGB
    GLint format = GL_RGBA16F;          // Format of the texture, e.g. GL_RGBA16F or GL_RGB
    GLenum type = GL_FLOAT;             // Type of the texture
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

    ~FrameBuffer();

    /**
     * @brief Updates the size of the FrameBuffer (attachments). 
     * @param width Width of the framebuffer.
     * @param height Height of the framebuffer.
     */
    virtual void setSize(
        const int width, 
        const int height
    );

    /** 
     * @brief Binds the framebuffer for rendering.
     * @see glBindFramebuffer()
     */
    virtual void bind() const;

    /** @return fbo handle (Frame Buffer Object) */
    GLuint getId() const { return m_fbo; }

    int getWidth() const { return m_width; }

    int getHeight() const { return m_height; }

    /** Shortcut for getting the first color attachment / texture */
    const GLint getTexture() const { 
        return getColorAttachment(GL_COLOR_ATTACHMENT0).texture;
    }

    /**
     * @brief Get a color attachment by its attachment point.
     * @param attachment Attachment point, e.g. GL_COLOR_ATTACHMENT0.
     * @return Texture attached to the framebuffer at the given attachment point.
     * @throws std::out_of_range if the attachment is out of range (greater than GL_COLOR_ATTACHMENT15).
     */
    const FboAttachment& getColorAttachment(GLenum attachment) const;

    /**
     * @brief Adds a color attachment to the framebuffer.
     * @param attachment Attachment point, e.g. GL_COLOR_ATTACHMENT0.
     * @param internalformat Internal format of the texture, e.g. GL_RGBA16F.
     * @param format Format of the texture, e.g. GL_RGBA.
     * @param type Type of the texture, e.g. GL_FLOAT (HRD) or GL_UNSIGNED_INT (normal).
     * @param andBindUnbind Whether to bind/unbind the framebuffer when adding the attachment.
     * @see glTexImage2D(), glFramebufferTexture2D()
     */
    void addColorAttachment(
        GLenum attachment,
        GLint internalformat = GL_RGBA16F,
        GLint format = GL_RGBA,
        GLenum type = GL_FLOAT,
        bool andBindUnbind = true
    );

protected:

    FrameBufferSettings m_settings;

    int m_width = 100;

    int m_height = 100;

    /** 
     * @brief Color attachments of the framebuffer.
     * [<GL_COLOR_ATTACHMENTX Attachment>, <FboAttachment>]
     */
    // std::unordered_map<GLenum, Ref<Texture>> m_colorAttachments;
    std::unordered_map<GLenum, FboAttachment> m_colorAttachments;

    /** Reference to the OpenGL FrameBuffer ID */
	GLuint m_fbo = 0;

    /** Reference to the OpenGL RenderBuffer ID */
	GLuint m_rbo = 0;

private:
    /**
     * @brief (internal) List of color attachments for glDrawBuffers().
     */
    std::vector<GLenum> m_drawBuffers; 
};
