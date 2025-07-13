#pragma once

#include "common/ref.h"
#include "common/gl.h"

#include "component/Camera.h"
#include "component/Mesh.h"
#include "gfx/RenderPass.h"

struct RendererConfig {
	bool wireframe = false;
	bool useRenderpass = true;
};

class Renderer {
public:

 	RendererConfig m_config;

	Renderer(RendererConfig config = {});

	Renderer(const Renderer&) = delete;
	~Renderer();

	/**
	 * @brief Sets the viewport size using `glViewport()`. 
	 */
	void setSize(
		GLint x,
		GLint y,
		GLsizei width,
		GLsizei height
	);

	/**
	 * @brief Sets the viewport size using `glViewport()`. 
	 */
	void setSize(
		const int width, 
		const int height
	);

	/**
	 * @brief Draw a set of meshes with the given camera.
	 */
	void draw(
		std::vector<Ref<Mesh>> meshes, 
		Ref<Camera> camera
	);

	/**
	 * @brief Draw a single mesh with the given camera.
	 */
	void drawMesh(
		Ref<Mesh> mesh, 
		Ref<Camera> camera
	);

	void clear();
	
private:

	/**
	 *  @brief fullscreen quad for render passes. 
	 */
	Mesh m_fullscreenQuad;
	
	// Material m_renderPassMaterial;

	std::vector<Ref<RenderPass>> m_renderPasses = {};

};

/**
 * @brief OpenGL debug message callback function.
 * @note Only works in OpenGL version 4.3+
 */
static void GlDebugMsg(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar *message,
	const void *userParam
);
