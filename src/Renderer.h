#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>

/** 
 * Renderer class responsible for rendering graphics using SDL2.
 */
class Renderer {
public:
    /** Initializes the renderer and sets up the necessary resources. */
    void initialize();

    /** Clears the current rendering target. */
    void clear();

    /** Draws the current frame to the screen. */
    void draw();

    /** Cleans up resources used by the renderer. */
    void cleanup();

private:
    SDL_Renderer* sdlRenderer; /**< Pointer to the SDL renderer. */
};

#endif // RENDERER_H