#pragma once

#include "common/gl.h"
#include "gfx/FrameBuffer.h"
#include "gfx/RenderPass.h"

/**
 * @brief Smear pass, blurs the (edges of) the screen
 */
class SmearPass : public RenderPass {
public:

    SmearPass()
        : RenderPass(
            Material(
                ref<Shader>("Basic.vert", "renderpass/Smear.frag"), {
                    { "u_smearAlpha", ref<Uniform<float>>(0.1f) },
                    { "u_smearStart", ref<Uniform<float>>(0.0f) },
                    { "u_smearEnd", ref<Uniform<float>>(0.5f) },
                }
            ),
            RenderPassConfig {
                .autoClear = false
            },
            FrameBufferSettings {
                .useHdr = true,
                .attachDefaultColorAttachment = true,
                .attachRenderBufferObject = false,
            }
        )
    {}
};
