#ifndef GAME_H
#define GAME_H

#include "Renderer.h"
#include "InputHandler.h"

/** 
 * Game class manages the game state, updates, and rendering.
 */
class Game {
public:

    Renderer renderer;
    InputHandler inputHandler;

    /** Initializes the game and its resources. */
    void start();

    /** Updates the game state. */
    void update();

    /** Renders the game graphics. */
    void render();
};

#endif // GAME_H