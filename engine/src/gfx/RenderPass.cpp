
#include "gfx/RenderPass.h"
#include "common/gl.h"

RenderPass::RenderPass(
    Material material, 
    RenderPassConfig config, 
    FrameBufferSettings frameBufferSettings
) 
    : m_material(ref<Material>(material)), m_config(config), m_frameBuffer({
        FrameBuffer(frameBufferSettings)
    })
{}

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
        /* Note: if there are no color attachments, this could throw GL_INVALID_OPERATION */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    CHECK_GL_ERROR();

    /* Bind shader */
    m_material->m_shader->bind();

    /* Manually bind textures */
    m_material->setUniform("u_readBuffer", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, readBuffer.getTexture());

    /* Bind the material - only update uniforms, without textures */
    m_material->bind(false);

    CHECK_GL_ERROR();
}

void RenderPass::draw(
    Mesh& fullscreenQuad
) {
    _drawQuad(fullscreenQuad, m_material);
}

void RenderPass::_drawQuad(
    Mesh& fullscreenQuad,
    Ref<Material> material
) {
    fullscreenQuad.setMaterial(material);
    // fullscreenQuad.m_geometry->bind(); // Already done in Renderer
    fullscreenQuad.m_material->bind(false);

    glDrawElements(
        GL_TRIANGLES, 
        fullscreenQuad.m_geometry->m_indexBuffer->getCount(), 
        GL_UNSIGNED_INT, 
        0
    );
}
