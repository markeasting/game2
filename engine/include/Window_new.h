#pragma once

#include <SDL2/SDL.h> 

struct WindowConfig {
    const char* windowTitle = "MOI";
    uint16_t windowWidth = 2560;
    uint16_t windowHeight = 1440;
    bool fullscreen = false;
    bool vsync = true;
};

/**
 * @class Window
 * @brief Encapsulates SDL window creation and management.
 */
class Window {
public:

    WindowConfig m_config;

    /**
     * @brief Constructor to initialize the SDL window.
     * @param title The title of the window.
     * @param width The width of the window.
     * @param height The height of the window.
     */
    Window(WindowConfig m_config);

    /**
     * @brief Destructor to clean up resources.
     */
    ~Window();

    /**
     * @brief Gets the SDL window instance.
     * @return Pointer to the SDL_Window.
     */
    SDL_Window* getSDLWindow() const;

    /**
     * @brief Swaps the front and back buffers.
     */
    void swapBuffers();

    /**
     * @brief Gets the framebuffer width.
     * @return framebuffer width.
     */
    int getFrameBufferWidth() const { return m_frameBufferWidth; }

    /**
     * @brief Gets the framebuffer height.
     * @return framebuffer height.
     */
    int getFrameBufferHeight() const { return m_frameBufferHeight; }

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
