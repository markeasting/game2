
#include "gfx/RenderPass.h"

RenderPass::RenderPass(
    Material material, 
    RenderPassConfig config, 
    FrameBufferSettings frameBufferSettings
) 
    : m_material(material), m_config(config), m_frameBuffer({
        FrameBuffer(frameBufferSettings)
    })
{}

/**
    * @brief Updates the size of the render pass framebuffer
    */
void RenderPass::setSize(
    GLint x,
    GLint y,
    GLsizei width,
    GLsizei height
) {
    m_frameBuffer.setSize(width, height);
};

void RenderPass::bind(
    const FrameBuffer& readBuffer
) {
    /* Bind the render pass draw framebuffer */
    // glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_frameBuffer.getId());
    m_frameBuffer.bind();

    /* Set up OpenGL state */
    if (m_config.autoClear) {
        glClearColor(
            m_config.clearColor.r,
            m_config.clearColor.g,
            m_config.clearColor.b,
            m_config.clearColor.a
        );
        glClear(GL_COLOR_BUFFER_BIT); // | GL_DEPTH_BUFFER_BIT
    }

    /* Bind the material / shader */
    m_material.bind();

    /* Bind each read buffer to a texture unit */
    // for (size_t i = 0; i < readBuffers.size(); ++i) {
    //     const FrameBuffer& readBuffer = readBuffers[i];
    //     // m_material.setUniform("u_readBuffer" + std::to_string(i), i); // Texture unit i
    //     glActiveTexture(GL_TEXTURE0 + i);
    //     glBindTexture(GL_TEXTURE_2D, readBuffer.getTexture());
    // }

    m_material.setUniform("u_readBuffer", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, readBuffer.getTexture());
}

void RenderPass::draw(
    Mesh& fullscreenQuad
) {
    _drawQuad(fullscreenQuad, m_material);
}

void RenderPass::_drawQuad(
    Mesh& fullscreenQuad,
    Material& material
) {
    fullscreenQuad.setMaterial(material);
    fullscreenQuad.bind(); // @todo move bind to Renderer?

    glDrawElements(GL_TRIANGLES, fullscreenQuad.m_geometry->m_indexBuffer->getCount(), GL_UNSIGNED_INT, 0);
}
