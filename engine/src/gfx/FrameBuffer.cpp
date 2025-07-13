
#include "gfx/FrameBuffer.h"
#include "common/gl.h"
#include "gfx/Texture.h"
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
        addColorAttachment(
            GL_COLOR_ATTACHMENT0, 
            // Note: using RGBA will give different results
            settings.useHdr ? GL_RGB16F : GL_RGB,
            settings.useHdr ? GL_RGB : GL_RGB,
            GL_UNSIGNED_BYTE, 
            false
        );
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
        CHECK_GL_ERROR();
    }
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void FrameBuffer::addColorAttachment(
    GLenum attachment,
    GLint internalformat,
    GLint format,
    GLenum type,
    bool andBindUnbind
) {

    assert(m_colorAttachments.find(attachment) == m_colorAttachments.end() && 
           "Color attachment already exists in FrameBuffer.");

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

    /* Wrapping - not needed */
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

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

    glFramebufferTexture2D(
        GL_FRAMEBUFFER, // could also be GL_DRAW_ or GL_READ_
        attachment, // GL_COLOR_ATTACHMENT0,
        GL_TEXTURE_2D,
        fboAttachment.texture,
        0
    );

    CHECK_GL_ERROR();

    /* @todo maybe increment, GL_COLOR_ATTACHMENT0++ */
    m_colorAttachments[attachment] = fboAttachment;
    m_drawBuffers.push_back(fboAttachment.attachment);

    if (andBindUnbind)
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::setSize(
    const int width, 
    const int height
) {
    this->invalidate();

    m_width = width;
    m_height = height;

    /* Bind */
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

    if (m_colorAttachments.empty()) {
        throw std::runtime_error("No color attachments found in FrameBuffer.");
    }

    for (const auto& [unit, attachment] : m_colorAttachments) {
        // tex->load(width, height, GL_RGBA16F, nullptr);

        glBindTexture(GL_TEXTURE_2D, attachment.texture);
        glTexImage2D(
            GL_TEXTURE_2D, 
            0, 
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
        // CHECK_GL_ERROR();
    }

    /* Check if everything is bound correctly */
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE)
    {
        printf("[FrameBuffer] error: %d\n", status);
        throw std::runtime_error("Framebuffer is not complete.");
    }

    /* Unbind the created fbo/rbo (rebind default 0) to make sure we're not using the wrong framebuffer */
    /* If this results in a blank screen, it could be that the default framebuffer has a diffent ID! */
    /* In that case, get the default ID by calling glGetIntegerv(GL_FRAMEBUFFER_BINDING, &oldFBO); */
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    printf("[FrameBuffer] Created framebuffer %d with renderbuffer %d\n", 
        m_fbo, 
        m_rbo
    );
}

void FrameBuffer::invalidate() {
    if (m_fbo != 0) {
        // printf("[FrameBuffer] Invalidating framebuffer %d\n", m_fbo);

        glBindFramebuffer(GL_FRAMEBUFFER, 0); 
        glBindRenderbuffer(GL_RENDERBUFFER, 0);

        /* Deleting is not really needed, we only need to update the attachments */
        // glDeleteFramebuffers(1, &m_fbo);
        // glDeleteRenderbuffers(1, &m_rbo);
    }
}

void FrameBuffer::bind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

    glDrawBuffers(m_drawBuffers.size(), m_drawBuffers.data());

    /* Set new viewport if you want to render to a specific part of the screen */
    // glViewport(w, h)
}
