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
                    { "u_smearAlpha", uniform(0.3f) },
                    { "u_smearStart", uniform(0.2f) },
                    { "u_smearEnd", uniform(0.6f) },
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
