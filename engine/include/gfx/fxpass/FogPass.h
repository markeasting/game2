#pragma once

#include "common/gl.h"
#include "gfx/FrameBuffer.h"
#include "gfx/RenderPass.h"

class FogPass : public RenderPass {
public:

    FogPass()
        : RenderPass(
            Material(
                ref<Shader>("Basic.vert", "renderpass/Fog.frag"), {
                    { "u_fogStart", uniform(5.0f) },
                    { "u_fogEnd", uniform(30.0f) },
                    { "u_fogColor", uniform(vec4(1.0f, 1.0f, 1.0f, 0.5f)) },
                }
            ),
            RenderPassConfig {
                .autoClear = true
            },
            FrameBufferSettings {
                .attachDefaultColorAttachment = false,
                .attachRenderBufferObject = false,
            }
        )
    {
        m_frameBuffer.addAttachment(
            GL_COLOR_ATTACHMENT0,
            GL_RGBA16F,
            GL_RGBA,
            GL_FLOAT
        );

        // Pass through buffer
        // m_frameBuffer.addAttachment(
        //     GL_COLOR_ATTACHMENT1,
        //     GL_RED,
        //     GL_RED,
        //     GL_FLOAT
        // );
    }

    void bind(
        const FrameBuffer& readBuffer
    ) override {
        RenderPass::bind(readBuffer);

        m_material->setUniform("u_depth", 1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, readBuffer.getDepthTexture());
    }
};
