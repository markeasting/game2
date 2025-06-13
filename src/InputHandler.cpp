#include "InputHandler.h"

/** 
 * InputHandler class implementation for processing user input.
 */

/** 
 * Processes user input from the keyboard and mouse.
 * 
 * @param event The SDL event to process.
 */
void InputHandler::processInput(const SDL_Event& event) {
    /* Check for quit event */
    if (event.type == SDL_QUIT) {
        // Handle quit event
    }

    /* Check for key down events */
    if (event.type == SDL_KEYDOWN) {
        // Handle key down events
    }

    /* Check for key up events */
    if (event.type == SDL_KEYUP) {
        // Handle key up events
    }

    /* Check for mouse motion events */
    if (event.type == SDL_MOUSEMOTION) {
        // Handle mouse motion events
    }
}

/** 
 * Checks if a specific key is pressed.
 * 
 * @param key The SDL key to check.
 * @return True if the key is pressed, false otherwise.
 */
bool InputHandler::isKeyPressed(SDL_Keycode key) {
    const Uint8* state = SDL_GetKeyboardState(NULL);
    return state[SDL_GetScancodeFromKey(key)];
}