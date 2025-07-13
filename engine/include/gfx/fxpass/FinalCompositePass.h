#pragma once

#include "common/gl.h"
#include "gfx/FrameBuffer.h"
#include "gfx/RenderPass.h"

/**
 * @brief Final composite pass, combines all render passes into a single output.
 * This pass is used to render the final scene after all other passes have been applied.
 */
class FinalCompositePass : public RenderPass {
public:

    FinalCompositePass()
        : RenderPass(
            Material(
                ref<Shader>("Basic.vert", "renderpass/Final.frag"), {
                    // No uniforms needed for the final composite pass
                }
            ),
            RenderPassConfig {},
            FrameBufferSettings {
                .attachDefaultColorAttachment = true,
                .attachRenderBufferObject = false,
            }
        )
    {}
};
