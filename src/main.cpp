// // #include <iostream>
// #include "Game.h"

// /** Entry point of the game */
// int main(int argc, char** argv) {
//     // Create a Game instance
//     Game game;

//     // Initialize the game
//     // if (!game.initialize()) {
//     //     std::cerr << "Failed to initialize the game." << std::endl;
//     //     return -1;
//     // }

//     // Start the game loop
//     game.start();

//     return 0;
// }

#include <SDL2/SDL.h>
#include <iostream>

int main(int argc, char* argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Unable to init SDL: " << SDL_GetError() << '\n';
        return 1;
    }
    SDL_Window* window = SDL_CreateWindow("Hello SDL",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800, 600, 0
    );

    if (!window) {
        std::cerr << "Unable to create window: " << SDL_GetError() << '\n';
        SDL_Quit();
        return 1;
    }
    
    SDL_Delay(2000);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
