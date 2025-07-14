
#include "gfx/FrameBuffer.h"
#include "common/gl.h"
#include <cassert>
#include <cstdio>
#include <stdexcept>

FrameBuffer::FrameBuffer(FrameBufferSettings settings) 
    : m_settings(settings) 
{
    /* Generate OpenGL resources */
    glGenFramebuffers(1, &m_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

    if (settings.attachDefaultColorAttachment) {
        addAttachment(
            GL_COLOR_ATTACHMENT0, 
            // Note: using RGBA will give different results
            settings.useHdr ? GL_RGB16F : GL_RGB,
            settings.useHdr ? GL_RGB : GL_RGB,
            GL_UNSIGNED_BYTE, 
            false
        );

        CHECK_GL_ERROR_THROW();
    }

    if (settings.attachRenderBufferObject) {
        
        printf("[FrameBuffer] Attaching render buffer object for depth and stencil.\n");
        
        /* Create the render buffer */
        glGenRenderbuffers(1, &m_rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, m_rbo); 
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_width, m_height);
        glFramebufferRenderbuffer(
            GL_FRAMEBUFFER,
            GL_DEPTH_STENCIL_ATTACHMENT,
            GL_RENDERBUFFER,
            m_rbo
        );
        
        CHECK_GL_ERROR_THROW();
    }
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

const FboAttachment& FrameBuffer::getAttachment(GLenum attachment) const {

    // printf("[FrameBuffer] Getting color attachment %d\n", attachment);

    // Slow
    // GLint maxColorAttachments;
    // glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &maxColorAttachments);
    
    // GLenum attach = GL_COLOR_ATTACHMENT0 + attachment;

    if (attachment < GL_COLOR_ATTACHMENT0 || attachment > GL_STENCIL_ATTACHMENT) {
        printf("[FrameBuffer] Error: Attachment %d is out of range\n", attachment);
        throw std::out_of_range("[FrameBuffer] ERR");
    }

    if (m_attachments.count(attachment)) {
        return m_attachments.at(attachment);
    }
    
    throw std::out_of_range("[FrameBuffer] Attachment not found");
    // return m_colorAttachments.at(attachment);
}

const FboAttachment& FrameBuffer::addAttachment(
    GLenum attachment,
    GLint internalformat,
    GLint format,
    GLenum type,
    bool andBindUnbind
) {

    assert(m_attachments.find(attachment) == m_attachments.end() && 
        "Attachment already exists in FrameBuffer.");

    if (andBindUnbind) 
        glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

    FboAttachment fboAttachment = {
        .attachment = attachment,
        .target = GL_TEXTURE_2D,
        .texture = 0, // Will be set after glGenTextures
        .level = 0,
        .internalformat = internalformat,
        .format = format,
        .type = type
    };

    glGenTextures(1, &fboAttachment.texture);

    /* Bind */
    glBindTexture(GL_TEXTURE_2D, fboAttachment.texture);

    /* Filtering */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    /* Wrapping (disable) */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    /* Anisotropic filtering - not needed */
    // GLfloat max_anisotropy;
    // glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &max_anisotropy);
    // glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, std::min(max_anisotropy, 16.0f));

    /* Upload data */
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        internalformat, 
        m_width, 
        m_height, 
        0, 
        format,
        type, 
        nullptr
    );

    /* Attach the texture to the framebuffer */
    glFramebufferTexture2D(
        GL_FRAMEBUFFER, // could also be GL_DRAW_ or GL_READ_
        attachment,     // GL_COLOR_ATTACHMENT0,
        GL_TEXTURE_2D,
        fboAttachment.texture,
        0
    );

    CHECK_GL_ERROR_THROW();

    /* @todo maybe increment automatically, ie. GL_COLOR_ATTACHMENT0++ */
    m_attachments[attachment] = fboAttachment;

    /* Only add 'color' attachments to glDrawBuffers() */
    if (attachment < GL_DEPTH_ATTACHMENT) {
        m_drawBufferAttachments.push_back(fboAttachment.attachment);
    }

    glDrawBuffers(m_drawBufferAttachments.size(), m_drawBufferAttachments.data());

    if (andBindUnbind)
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

    CHECK_GL_ERROR_THROW();

    return m_attachments[attachment];
}

void FrameBuffer::setSize(
    const GLsizei width, 
    const GLsizei height
) {
    m_width = width;
    m_height = height;

    /* Bind */
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

    if (m_attachments.empty()) {
        throw std::runtime_error("No color attachments found in FrameBuffer.");
    }

    /* Update all FBO attachments */
    for (const auto& [unit, attachment] : m_attachments) {
        glBindTexture(GL_TEXTURE_2D, attachment.texture);
        glTexImage2D(
            attachment.target,
            attachment.level, 
            attachment.internalformat, 
            width, 
            height,
            0,
            attachment.format,
            attachment.type,
            nullptr
        );
    }

    /* Attach the rbo to the depth and stencil attachment of the framebuffer */
    if (m_settings.attachRenderBufferObject) {
        glBindRenderbuffer(GL_RENDERBUFFER, m_rbo); 
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
        CHECK_GL_ERROR_THROW();
    }

    /* Check if everything is bound correctly */
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE)
    {
        printf("[FrameBuffer] error: %d\n", status);
        throw std::runtime_error("Framebuffer is not complete.");
    }

    /* Unbind */
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void FrameBuffer::bind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

    /* Render to all attached render targets */
    /* Note: the state of glDrawBuffers is stored by the FBO */
    // glDrawBuffers(m_attachedTargets.size(), m_attachedTargets.data());
}

FrameBuffer::~FrameBuffer() {
    
    // printf("[FrameBuffer] Destroying FrameBuffer %d\n", m_fbo);

    /* Just destroy the OpenGL context and let it bleed out */
    // if (m_fbo) {
    //     glDeleteFramebuffers(1, &m_fbo);
    // }
    // if (m_rbo) {
    //     glDeleteRenderbuffers(1, &m_rbo);
    // }
    
    // for (const auto& [attachment, fboAttachment] : m_colorAttachments) {
    //     glDeleteTextures(1, &fboAttachment.texture);
    // }
}
