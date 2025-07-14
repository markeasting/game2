#pragma once

#include "common/gl.h"
#include "gfx/FrameBuffer.h"
#include "gfx/RenderPass.h"

/**
 * @brief Pre-pass for the bloom effect.
 * This pass extracts bright areas from the scene to be used in the bloom effect.
 * 
 * Uses a framebuffer with two color attachments:
 * - GL_COLOR_ATTACHMENT0: FragColor:   The original scene color.
 * - GL_COLOR_ATTACHMENT1: BrightColor: Brightest areas
 * 
 * Then it blurs the BrightColor buffer and composites them, 
 * using a ping-pong buffer technique.
 */
class BloomPass : public RenderPass {
public:

    FrameBuffer m_pingpong1;
    FrameBuffer m_pingpong2;

    Material m_blurMaterial = Material(
        ref<Shader>("Basic.vert", "renderpass/GaussianBlur.frag"), {
            { "u_horizontal", ref<Uniform<bool>>(false) },
        }
    );

    Material m_compositeMaterial = Material(
        ref<Shader>("Basic.vert", "renderpass/BloomComposite.frag")
    );

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
        /* HDR attachment (FragColor) */
        m_frameBuffer.addColorAttachment(
            GL_COLOR_ATTACHMENT0,
            GL_RGBA16F,
            GL_RGBA,
            GL_FLOAT
        );
        
        /* Threshold attachment (BrightColor) */
        m_frameBuffer.addColorAttachment(
            GL_COLOR_ATTACHMENT1,
            GL_RGBA16F,
            GL_RGBA,
            GL_FLOAT
        );

        /* Blur ping/pong buffers */
        m_pingpong1.addColorAttachment(
            GL_COLOR_ATTACHMENT0
        );

        m_pingpong2.addColorAttachment(
            GL_COLOR_ATTACHMENT0
        );
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

    void bind(
        const FrameBuffer& readBuffer
    ) override {
        RenderPass::bind(readBuffer);
    }

    GLuint pingpongFBO[2];
    unsigned int pingpongBuffers[2];

    void draw(
        Mesh& fullscreenQuad
    ) override {

        /* 
         * Draw the initial frame into two targets: 
         * - GL_COLOR_ATTACHMENT0: default color (original frame)
         * - GL_COLOR_ATTACHMENT1: thresholded bright areas
         */
        _drawQuad(fullscreenQuad, m_material);

        /* Copy the contents of the bright areas to the first pingpong buffer */
        assert(m_pingpong1.getWidth() == m_frameBuffer.getWidth());
        glBindFramebuffer(GL_READ_FRAMEBUFFER, m_frameBuffer.getId());
        glReadBuffer(GL_COLOR_ATTACHMENT1); // Read from the BrightColor attachment
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_pingpong1.getId());
        glDrawBuffer(GL_COLOR_ATTACHMENT0);  // Write to attachment 0   
        glBlitFramebuffer(
            0, 0, m_frameBuffer.getWidth(), m_frameBuffer.getHeight(), // src rect
            0, 0, m_pingpong1.getWidth(), m_pingpong1.getHeight(), // dst rect
            GL_COLOR_BUFFER_BIT,       // What data to copy
            GL_NEAREST                 // Filtering
        );
        
        /* Blurring state using pingpong buffers */
        pingpongFBO[0] = m_pingpong1.getId();
        pingpongFBO[1] = m_pingpong2.getId();
        pingpongBuffers[0] = m_pingpong1.getTexture();
        pingpongBuffers[1] = m_pingpong2.getTexture();

        bool horizontal = false;
        int amount = 10;

        for (unsigned int i = 0; i < amount; i++)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[!horizontal]); // Yes, this must be !horizontal.  
            
            m_blurMaterial.setUniform("u_horizontal", horizontal);
            m_blurMaterial.setUniform("u_readBuffer", 0);
            
            glBindTexture(
                GL_TEXTURE_2D, pingpongBuffers[horizontal]
            );
            
            _drawQuad(fullscreenQuad, m_blurMaterial);

            horizontal = !horizontal;
        }
        
        /* Now we composite the blurred result with the underlying colors */
        m_frameBuffer.bind();
        
        m_compositeMaterial.bind();
        m_compositeMaterial.setUniform("u_readBuffer", 0);
        m_compositeMaterial.setUniform("u_bloomBuffer", 1); // Texture unit
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(
            GL_TEXTURE_2D, m_frameBuffer.getTexture()
        );
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(
            GL_TEXTURE_2D, pingpongBuffers[horizontal]
        );

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

// class BloomBlurPass : public RenderPass {
// public:

//     FrameBuffer m_secondBuffer;

//     GLuint pingpongFBO[2];
//     unsigned int pingpongBuffers[2];

//     BloomBlurPass()
//         : RenderPass(
//             Material(
//                 ref<Shader>("Basic.vert", "renderpass/BloomBlur.frag"), {
//                     { "u_horizontal", ref<Uniform<bool>>(false) },
//                 }
//             ),
//             RenderPassConfig {
//                 .autoClear = true
//             },
//             FrameBufferSettings {
//                 .attachDefaultColorAttachment = false,
//                 .attachRenderBufferObject = false,
//             }
//         )
//     {
//         /* Buffer 1, for ping-pong */
//         m_frameBuffer.addColorAttachment(
//             GL_COLOR_ATTACHMENT0,
//             GL_RGBA16F,
//             GL_RGBA,
//             GL_FLOAT
//         );

//         /* Buffer 2, for ping-pong */
//         m_secondBuffer.addColorAttachment(
//             GL_COLOR_ATTACHMENT0,
//             GL_RGBA16F,
//             GL_RGBA,
//             GL_FLOAT
//         );
//         // m_frameBuffer.push_back(m_secondBuffer);
//     }

//     void bind(
//         const FrameBuffer& readBuffer
//     ) override {
//         RenderPass::bind(readBuffer);

//         // // Assuming the previous pass is BloomPrePass,
//         // // we need to copy its contents of the HDR attachment (GL_COLOR_ATTACHMENT1)
//         // // to the first pingpong buffer (m_frameBuffer).
//         // glBindFramebuffer(GL_READ_FRAMEBUFFER, readBuffer.getId());
//         // glReadBuffer(GL_COLOR_ATTACHMENT1); // Read from the BrightColor attachment

//         // glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_frameBuffer.getId());
//         // glDrawBuffer(GL_COLOR_ATTACHMENT0);  // Write to attachment 0

//         // glBlitFramebuffer(
//         //     0, 0, readBuffer.getWidth(), readBuffer.getHeight(), // src rect
//         //     0, 0, m_frameBuffer.getWidth(), m_frameBuffer.getHeight(), // dst rect
//         //     GL_COLOR_BUFFER_BIT,       // What data to copy
//         //     GL_NEAREST                 // Filtering
//         // );

//         // m_material.setUniform("u_readBuffer", 0);
//         // glActiveTexture(GL_TEXTURE0);
//         // glBindTexture(GL_TEXTURE_2D, m_frameBuffer.getTexture());
//     }

//     void draw(
//         Mesh& quad
//     ) override {

//         RenderPass::draw(quad);

//         // pingpongFBO[0] = m_frameBuffer.getId();
//         // pingpongFBO[1] = m_secondBuffer.getId();
//         // pingpongBuffers[0] = m_frameBuffer.getTexture();
//         // pingpongBuffers[1] = m_secondBuffer.getTexture();

//         // bool horizontal = true, first_iteration = true;
//         // int amount = 10;

//         // for (unsigned int i = 0; i < amount; i++)
//         // {
//         //     glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[horizontal]); 
            
//         //     m_material.setUniform("u_horizontal", horizontal);
            
//         //     glBindTexture(
//         //         GL_TEXTURE_2D, pingpongBuffers[!horizontal]
//         //     ); 
            
//         //     RenderPass::draw(quad);

//         //     horizontal = !horizontal;

//         //     if (first_iteration)
//         //         first_iteration = false;
//         // }
//     }

// };


// class BloomCompositePass : public RenderPass {
// public:

// };
