#pragma once

#include "common/gl.h"
#include "gfx/FrameBuffer.h"
#include "gfx/RenderPass.h"

/**
 * Bloom effect.
 * 
 * Uses a framebuffer with two color attachments:
 * - GL_COLOR_ATTACHMENT0: FragColor:   The original scene color.
 * - GL_COLOR_ATTACHMENT1: BrightColor: Brightest areas
 * 
 * Then it blurs the BrightColor buffer, and then composites it 
 * with the original.
 */
class BloomPass : public RenderPass {
public:

    Ref<Material> m_blurMaterial = nullptr;
    Ref<Material> m_compositeMaterial = nullptr;

    FrameBuffer m_pingpong1;
    FrameBuffer m_pingpong2;

    GLuint pingpongFBO[2];
    GLuint pingpongBuffers[2];

    BloomPass()
        : RenderPass(
            Material(
                ref<Shader>("Basic.vert", "renderpass/Bloom.frag"), {
                    { "u_bloomThreshold", ref<Uniform<float>>(1.1f) },
                    // { "u_bloomThreshold", ref<Uniform<float>>(0.0f) },
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

        m_blurMaterial = ref<Material>(
            Material(ref<Shader>("Basic.vert", "renderpass/GaussianBlur.frag"), {
                { "u_horizontal", ref<Uniform<bool>>(false) },
            })
        );

        m_compositeMaterial = ref<Material>(
            Material(ref<Shader>("Basic.vert", "renderpass/BloomComposite.frag"))
        );

        /* HDR attachment (FragColor) */
        m_frameBuffer.addAttachment(
            GL_COLOR_ATTACHMENT0,
            GL_RGBA16F,
            GL_RGBA,
            GL_FLOAT
        );
        
        /* Threshold attachment (BrightColor) */
        m_frameBuffer.addAttachment(
            GL_COLOR_ATTACHMENT1,
            GL_RGBA16F,
            GL_RGBA,
            GL_FLOAT
        );

        /* Blur ping/pong buffers */
        m_pingpong1.addAttachment(
            GL_COLOR_ATTACHMENT0
        );

        m_pingpong2.addAttachment(
            GL_COLOR_ATTACHMENT0
        );

        pingpongFBO[0] = m_pingpong1.getId();
        pingpongFBO[1] = m_pingpong2.getId();
        pingpongBuffers[0] = m_pingpong1.getTexture();
        pingpongBuffers[1] = m_pingpong2.getTexture();
    }

    void setSize(
        GLint x,
        GLint y,
        GLsizei width,
        GLsizei height
    ) override {
        m_frameBuffer.setSize(width, height);
        m_pingpong1.setSize(width, height);
        m_pingpong2.setSize(width, height);
    }

    // void bind(
    //     const FrameBuffer& readBuffer
    // ) override {
    //     RenderPass::bind(readBuffer);
    // }

    void draw(
        Mesh& fullscreenQuad
    ) override {
        
        /* 
         * Stage 1: thresholding
         * Draw the initial frame into two targets: 
         * - GL_COLOR_ATTACHMENT0: default color (original frame)
         * - GL_COLOR_ATTACHMENT1: thresholded bright areas
         */
        _drawQuad(fullscreenQuad, m_material);

        /* 
         * Stage 2: blurring
         * Now we blur the bright areas using a ping-pong technique.
         * The first pass reads from GL_COLOR_ATTACHMENT1 and writes to pingpongBuffers[0].
         * The second pass reads from the first pingpong buffer, etc.
         */
        int amount = 10;
        bool horizontal = false;

        for (unsigned int i = 0; i < amount; i++)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[!horizontal]); // Yes, this must be !horizontal.  
            
            m_blurMaterial->setUniform("u_horizontal", horizontal);
            
            /* On the first pass, bind the 'bright' texture (GL_COLOR_ATTACHMENT1) */
            glBindTexture(
                GL_TEXTURE_2D, i == 0 
                    ? m_frameBuffer.getAttachment(GL_COLOR_ATTACHMENT1).texture
                    : pingpongBuffers[horizontal]
            );
            
            _drawQuad(fullscreenQuad, m_blurMaterial);

            horizontal = !horizontal;
        }

        /** 
         * Stage 3: compositing
         * Now we composite the blurred result with the underlying colors 
         * by binding the main framebuffer and using the composite shader.
         */

        m_frameBuffer.bind();
        
        m_compositeMaterial->bind();
        m_compositeMaterial->setUniform("u_readBuffer", 0);
        m_compositeMaterial->setUniform("u_bloomBuffer", 1); // Texture unit 1

        /* Bind u_readBuffer to texture unit 1 */
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(
            GL_TEXTURE_2D, m_frameBuffer.getTexture()
        );

        /* Bind u_bloomBuffer to texture unit 1 */
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(
            GL_TEXTURE_2D, pingpongBuffers[horizontal]
        );

        /* Run the BloomComposite shader */
        _drawQuad(fullscreenQuad, m_compositeMaterial);

        // /* Debug: copy the blurred result back to the main framebuffer */
        // glBindFramebuffer(GL_READ_FRAMEBUFFER, m_pingpong1.getId());
        // glReadBuffer(GL_COLOR_ATTACHMENT0); // Read from the blurred attachment
        // glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_frameBuffer.getId());
        // glDrawBuffer(GL_COLOR_ATTACHMENT0);  // Write to the main color attachment
        // glBlitFramebuffer(
        //     0, 0, m_pingpong1.getWidth(), m_pingpong1.getHeight(), // src rect
        //     0, 0, m_frameBuffer.getWidth(), m_frameBuffer.getHeight(), // dst rect
        //     GL_COLOR_BUFFER_BIT,       // What data to copy
        //     GL_NEAREST                 // Filtering
        // );
    }
};
