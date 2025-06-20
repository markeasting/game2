#pragma once

#include "common/ref.h"
#include "common/gl.h"

#include "obj/Mesh.h"
#include "gfx/Shader.h"

// #include "scene/Scene.h"
#include "core/Camera.h"

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

	void setSize(int width, int height);

	void draw(std::vector<Ref<Mesh>> meshes, Ref<Camera> camera);
	void clear();
	
private:
	void drawMesh(Ref<Mesh> mesh, Ref<Camera> camera);

	/**
	 * @todo make FullSceenQuad class for other render passes
	 */
	Mesh m_fullscreenQuad;
	
};

static void GlDebugMsg(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam);
