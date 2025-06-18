#include "Window.h"
#include <glad/glad.h>
#include <iostream>

Window::Window(WindowConfig m_config) {

    if (SDL_WasInit(SDL_INIT_VIDEO) == 0) {
        throw std::runtime_error(
            std::string("[Window] SDL was not initialized. Please call SDL_Init(SDL_INIT_VIDEO | ...)")
        );
    }

    m_window = SDL_CreateWindow(
        m_config.windowTitle,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        m_config.windowWidth, 
        m_config.windowHeight,
        SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI
    );

    if (!m_window) {
        throw std::runtime_error(
            std::string("[Window] Window could not be created: ") + SDL_GetError()
        );
    }

    initializeOpenGL();
}

Window::~Window() {
    SDL_GL_DeleteContext(m_glContext);
    SDL_DestroyWindow(m_window);
}

SDL_Window* Window::getSDLWindow() const {
    return m_window;
}

void Window::initializeOpenGL() {
    
    /* Context flags */
    int context_flags = SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG;

#ifdef DEBUG_BUILD
    context_flags |= SDL_GL_CONTEXT_DEBUG_FLAG;
#endif

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, context_flags);
    
    /* OpenGL attributes */
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    /* Platform-specific settings */
#ifdef __APPLE__
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
#else
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
#endif

    /* Create the OpenGL context */
    m_glContext = SDL_GL_CreateContext(m_window);

    if (!gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress)) {
        throw std::runtime_error("[Window] Failed to initialize OpenGL context");
    }

    SDL_GL_SetSwapInterval(m_config.vsync ? 1 : 0);
    SDL_GL_GetDrawableSize(m_window, &m_frameBufferWidth, &m_frameBufferHeight);

    std::cout << "[OpenGL] Device:  " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "[OpenGL] Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "[OpenGL] Window:  " << m_frameBufferWidth << " x " << m_frameBufferHeight << std::endl;
}
