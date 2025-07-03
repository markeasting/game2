#pragma once

#include <SDL2/SDL.h> 
#include <tuple>

struct WindowConfig {
    const char* windowTitle = "MOI";
    uint16_t windowWidth = 2560;
    uint16_t windowHeight = 1440;
    bool fullscreen = false;
    bool vsync = true;
};

/**
 * @brief Handles window creation and management using SDL2.
 */
class Window {
public:

    WindowConfig m_config;

    /**
     * @brief Constructor to initialize the game window.
     * @param m_config Window configuration / params.
     */
    Window(WindowConfig m_config);

    /**
     * @brief Destructor to clean up resources.
     */
    ~Window();

    /**
     * @brief Swaps the front and back buffers.
     */
    void swapBuffers();

    /**
     * @brief Handles window resize events, e.g. SDL_WINDOWEVENT_RESIZED.
     * @return A tuple containing the new framebuffer width and height.
     */
    std::tuple<int, int> handleResize();

    /**
     * @brief Gets the framebuffer width.
     */
    int getFrameBufferWidth() const { return m_frameBufferWidth; }

    /**
     * @brief Gets the framebuffer height.
     */
    int getFrameBufferHeight() const { return m_frameBufferHeight; }

    /**
     * @brief Gets the SDL OpenGL context.
     */
    SDL_GLContext getGLContext() const { return m_glContext; }

    /**
     * @brief Gets the SDL Window pointer
     */
    SDL_Window* getSDLWindow() const { return m_window; }

private:

    /** Pointer to the SDL window. */
    SDL_Window* m_window; 
    
    /** OpenGL context for the window. */
    SDL_GLContext m_glContext; 

    /** Framebuffer width */
    int m_frameBufferWidth = 0;

    /** Framebuffer height */
    int m_frameBufferHeight = 0;

    /**
     * @brief Initializes the OpenGL context.
     */
    void initializeOpenGL();
};
