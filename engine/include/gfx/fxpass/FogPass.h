#pragma once

#include "common/gl.h"
#include "gfx/FrameBuffer.h"
#include "gfx/RenderPass.h"

class FogPass : public RenderPass {
public:

    FogPass()
        : RenderPass(
            Material(
                ref<Shader>("Basic.vert", "renderpass/FogExp.frag"), {
                    { "u_fogColor", uniform(vec4(1.0f, 1.0f, 1.0f, 0.5f)) },
                    { "u_fogDensity", uniform(0.03f) },
                    // { "u_fogStart", uniform(5.0f) },
                    // { "u_fogEnd", uniform(30.0f) },
                }
            ),
            RenderPassConfig {
                .autoClear = true
            },
            FrameBufferSettings {
                .attachDefaultColorAttachment = true,
                .attachRenderBufferObject = false,
            }
        )
    {}

    void bind(
        const FrameBuffer& cameraBuffer,
        const FrameBuffer& prevPassBuffer
    ) override {
        RenderPass::bind(cameraBuffer, prevPassBuffer);

        m_material->setTexture(
            "u_depth", 
            GL_TEXTURE1, 
            cameraBuffer.getDepthTexture()
        );
    }
};
