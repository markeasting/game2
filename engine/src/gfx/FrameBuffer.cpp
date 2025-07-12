
#include "gfx/FrameBuffer.h"
#include <cstdio>
#include <stdexcept>

FrameBuffer::FrameBuffer() {
    glGenFramebuffers(1, &m_fbo);
    glGenRenderbuffers(1, &m_rbo);
}

void FrameBuffer::create(
    const int width, 
    const int height
) {

    this->invalidate();

    // glGenFramebuffers(1, &m_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

    /* Generate texture */
    // glGenTextures(1, &m_textureColorbuffer);
    // glBindTexture(GL_TEXTURE_2D, m_textureColorbuffer);
    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    // // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    // // float borderColor[] = { 1.0f, 0.0f, 0.0f, 1.0f };
    // // glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);  
    // glBindTexture(GL_TEXTURE_2D, 0); // done, we can unbind the texture 

    m_texture.load(width, height, GL_RGB, nullptr);

    /* Attach texture to the framebuffer object */
    // glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_textureColorbuffer, 0);  
    glFramebufferTexture2D(
        GL_FRAMEBUFFER,
        GL_COLOR_ATTACHMENT0,
        GL_TEXTURE_2D,
        m_texture.getId(),
        0
    );  

    /* Create render buffer object */
    // glGenRenderbuffers(1, &m_rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, m_rbo); 
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);  

    /* Attach the rbo to the depth and stencil attachment of the framebuffer */
    glFramebufferRenderbuffer(
        GL_FRAMEBUFFER,
        GL_DEPTH_STENCIL_ATTACHMENT,
        GL_RENDERBUFFER,
        m_rbo
    );

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

    printf("[FrameBuffer] Created framebuffer %d with texture %d and renderbuffer %d\n", 
        m_fbo, 
        m_texture.getId(), 
        m_rbo
    );
}

void FrameBuffer::invalidate() {
    if (m_fbo != 0) {
        printf("[FrameBuffer] Invalidating framebuffer %d\n", m_fbo);

        glBindFramebuffer(GL_FRAMEBUFFER, 0); 
        glBindRenderbuffer(GL_RENDERBUFFER, 0);

        /* Deleting is not really needed, we only need to update the attachments */
        // glDeleteFramebuffers(1, &m_fbo);
        // glDeleteRenderbuffers(1, &m_rbo);

        /** 
         * Also handled by texture load(). 
         * So, the texture still exists here, but will be reloaded once 
         * Texture::load() / create() is called. 
         */
        // m_texture.invalidate(); 
    }
}

void FrameBuffer::bind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

    /* Set new viewport if you want to render to a specific part of the screen */
    // glViewport(w, h)
}
