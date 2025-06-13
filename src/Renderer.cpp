// Renderer.cpp
#include "Renderer.h"
#include <SDL2/SDL.h>

/** 
 * Renderer class implementation for rendering graphics using SDL2.
 */

// SDL_Renderer pointer for rendering
SDL_Renderer* renderer = nullptr;

/**
 * Initializes the renderer and sets up the SDL context.
 * 
 * @param window Pointer to the SDL_Window to associate with the renderer.
 * @return true if initialization is successful, false otherwise.
 */
void Renderer::initialize() {
    auto window = SDL_CreateWindow("Game Window", 
        SDL_WINDOWPOS_UNDEFINED, 
        SDL_WINDOWPOS_UNDEFINED, 
        800, 600, 
        SDL_WINDOW_SHOWN
    );
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

/**
 * Clears the current rendering target with the specified color.
 */
void Renderer::clear() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

/**
 * Draws the current rendering target to the screen.
 */
void Renderer::draw() {
    SDL_RenderPresent(renderer);
}

/**
 * Cleans up the renderer and frees resources.
 */
void Renderer::cleanup() {
    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
}