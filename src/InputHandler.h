#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <SDL2/SDL.h>

/** 
 * @class InputHandler
 * @brief Handles user input from keyboard and mouse.
 */
class InputHandler {
public:
    /** 
     * @brief Processes the current input state.
     */
    void processInput(const SDL_Event& event);

    /** 
     * @brief Checks if a specific key is pressed.
     * @param key The key to check.
     * @return True if the key is pressed, false otherwise.
     */
    bool isKeyPressed(int key);
};

#endif // INPUT_HANDLER_H