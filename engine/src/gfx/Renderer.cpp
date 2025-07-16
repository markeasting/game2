
#include "gfx/Renderer.h"
#include "common/gl.h"

#include "gameobject/GameObject.h"
#include "component/Transform.h"

Renderer::Renderer(RendererConfig config): 
    m_config(config)
{
    
    GLint gl_major = 0, gl_minor = 0;
    glGetIntegerv(GL_MAJOR_VERSION, &gl_major);
    glGetIntegerv(GL_MINOR_VERSION, &gl_minor);

    if (gl_major > 4 && gl_minor >= 3) {
        glDebugMessageCallback(GlDebugMsg, nullptr);
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    }

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_CULL_FACE);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
    // https://learnopengl.com/Advanced-OpenGL/Stencil-testing
    // glEnable(GL_STENCIL_TEST);
    // glStencilMask(0xFF); // Enable writing to the stencil buffer
	// glStencilFunc(GL_EQUAL, 1, 0xFF);
    // glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP); // Default behavior

    /* Quad for render passes */
    m_fullscreenQuad.m_geometry = ref<Geometry>(
        std::vector<Vertex>{
            { vec3(  1, -1, 0 ), vec3( 0, 0, 1 ), vec2( 1, 0 ) },
            { vec3(  1,  1, 0 ), vec3( 0, 0, 1 ), vec2( 1, 1 ) },
            { vec3( -1, -1, 0 ), vec3( 0, 0, 1 ), vec2( 0, 0 ) },
            { vec3( -1,  1, 0 ), vec3( 0, 0, 1 ), vec2( 0, 1 ) },
        },
        std::vector<unsigned int>{ 0, 1, 2, 2, 1, 3 }
    );
    
    m_renderPasses = std::move(m_config.renderPasses); 
    
    /* Check errors */
    CHECK_GL_ERROR_THROW();
}

void Renderer::setSize(
    GLint x,
    GLint y,
    GLsizei width,
    GLsizei height
) {
    
    glViewport(x, y, width, height);

    for (const auto& renderPass : m_renderPasses) {
        renderPass->setSize(x, y, width, height);
    }
}

void Renderer::setSize(
    const int width,
    const int height
) {
    
    glViewport(0, 0, width, height);

    for (const auto& renderPass : m_renderPasses) {
        renderPass->setSize(0, 0, width, height);
    }
}

void Renderer::draw(std::vector<Ref<Mesh>> meshes, Ref<Camera> camera) {

    assert(camera != nullptr);

    /* static, evaluated only once */
    static bool useRenderpass = m_config.useRenderpass && !m_renderPasses.empty();

    if (useRenderpass) {
        camera->m_frameBuffer.bind();
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    
    /* Draw opaque meshes */
    for (auto const& mesh : meshes) {

        if (mesh->m_material->transparent)
            continue;

        this->drawMesh(mesh, camera);

        // @TODO check if unbinding VAO / shader / texture is required
        // Or if it has any impact on performance
        // mesh->unbind(); 
    }

    /* Draw transparent meshes - @todo sort back-to-front */
    for (auto const& mesh : meshes) {

        if (!mesh->m_material->transparent)
            continue;

        this->drawMesh(mesh, camera);

        // @TODO check if unbinding VAO / shader / texture is required
        // Or if it has any impact on performance
        // mesh->unbind(); 
    }

    if (useRenderpass) {

        /* Setup state */
        glDisable(GL_DEPTH_TEST); // Disable depth test for the final pass
        glDisable(GL_CULL_FACE); // Not needed for fullscreen quad
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Make sure 'wireframe' mode is disabled for the final pass
        
        /* Draw the fullscreen quad with each render pass */
        m_fullscreenQuad.m_geometry->bind();

        for (int i = 0; i < m_renderPasses.size(); i++) {
            
            const auto& renderPass = m_renderPasses[i];
            const auto& prevPass = m_renderPasses[i - 1];
            
            /*
             * On the first pass, use the camera's framebuffer.
             * After that, use the previous RenderPass framebuffer.
             */
            const auto& prevPassBuffer = (i == 0) 
                ? camera->m_frameBuffer 
                : m_renderPasses[i - 1]->m_frameBuffer;

            renderPass->bind(camera->m_frameBuffer, prevPassBuffer);
            renderPass->draw(m_fullscreenQuad);

            CHECK_GL_ERROR_THROW();
        }

        /* Blit the final render pass to the default frame buffer */
        const auto& finalRenderPass = m_renderPasses.back();
        const auto& finalRenderBuffer = finalRenderPass->m_frameBuffer;

        glBindFramebuffer(GL_READ_FRAMEBUFFER, finalRenderBuffer.getId());
        glReadBuffer(GL_COLOR_ATTACHMENT0); // Read from the first color attachment
        
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // default framebuffer
        glDrawBuffer(GL_BACK); // Write to BACK buffer - will cause GL_INVALID_OPERATION otherwise

        glBlitFramebuffer(
            0, 0, finalRenderBuffer.getWidth(), finalRenderBuffer.getHeight(), // src rect
            0, 0, finalRenderBuffer.getWidth(), finalRenderBuffer.getHeight(), // dst rect
            GL_COLOR_BUFFER_BIT,       // What data to copy 
            GL_NEAREST                 // Filtering 
        );

        CHECK_GL_ERROR_THROW();

        /* Reset state */
        glEnable(GL_DEPTH_TEST); 
        glEnable(GL_CULL_FACE);
    }

}

void Renderer::drawMesh(Ref<Mesh> mesh, Ref<Camera> camera) {

    // assert(mesh != nullptr);
    // assert(mesh->gameObject != nullptr && "Mesh must have a GameObject with a Transform component");

    mesh->bind();

    auto transform = mesh->gameObject->tryGetComponent<Transform>();

    /* If there's no transform component, we skip all projections */
    if (transform) {

        auto matrix = transform->getWorldPositionMatrix();

        // @TODO only used for skybox
        mesh->m_material->setUniform(
            "u_viewRotationMatrix", 
            glm::mat4(glm::mat3(camera->m_viewMatrix))
        );

        if (auto camTrans = camera->gameObject->tryGetComponent<Transform>()) {
            mesh->m_material->setUniform(
                "u_camPos", 
                camTrans->getPosition()
            );
        }

        // @TODO premature optimization is <...>
        // Maybe just go back to separate m * v * p
        mesh->m_material->setUniform(
            "u_projectionMatrix", 
            camera->m_projectionMatrix
        );

        mesh->m_material->setUniform(
            "u_modelMatrix", 
            matrix
        );

        mesh->m_material->setUniform(
            "u_modelViewMatrix", 
            transform->m_useProjectionMatrix 
                ? camera->m_viewMatrix * matrix
                : matrix
        );

        mesh->m_material->setUniform(
            "u_modelViewProjectionMatrix", 
            transform->m_useProjectionMatrix 
                ? camera->m_viewProjectionMatrix * matrix
                : matrix
        );

        /* Clear any errors from undefined uniform locations */
        CHECK_GL_ERROR();
    }

    if (m_config.wireframe || mesh->m_material && mesh->m_material->wireframe) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDisable(GL_CULL_FACE);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glEnable(GL_CULL_FACE);
    }

    // @TODO add support for instanced meshes using glDrawArraysInstanced and glDrawElementsInstanced
    if (mesh->m_geometry->hasIndices()) {
        glDrawElements(GL_TRIANGLES, mesh->m_geometry->m_indexBuffer->getCount(), GL_UNSIGNED_INT, 0);
    } else {
        glDrawArrays(GL_TRIANGLES, 0, mesh->m_geometry->m_vertexBuffer->getCount());
    }
}

void Renderer::clear() {
    glUseProgram(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

Renderer::~Renderer() {
    
}

static void GlDebugMsg(
    GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar *message,
    const void *userParam
) {
    switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH:
            std::cout << "[ERROR] " << message << std::endl;
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            std::cout << "[ERROR] " << message << std::endl;
            break;
        case GL_DEBUG_SEVERITY_LOW:
            std::cout << "[ERROR] " << message << std::endl;
            break;
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            // std::cout << "[DEBUG] " << message << std::endl;
            break;
    }
}

