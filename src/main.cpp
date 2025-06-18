#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <iostream>
#include "Window.h"

int main(int argc, char *argv[])
{
    try {
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) < 0) {
            throw std::runtime_error(
                std::string("SDL could not be initialized: ") + SDL_GetError()
            );
        }

        Window window({
            .windowTitle = "MOIII",
            .windowWidth = 1280,
            .windowHeight = 720,
            .fullscreen = false,
            .vsync = true
        });

        /* Event loop */
        bool quit = false;
        SDL_Event e;

        while (!quit) {
            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_KEYDOWN) {
                    std::cout << "Key pressed: " << e.key.keysym.sym << std::endl;
                } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                    std::cout << "Mouse button pressed: " << e.button.button << std::endl;
                } else if (e.type == SDL_WINDOWEVENT) {
                    if (e.window.event == SDL_WINDOWEVENT_CLOSE) {
                        quit = true;
                    }
                }

                if (e.type == SDL_QUIT) {
                    quit = true;
                }
            }
        }
    } catch (const std::exception &e) {
        std::cerr << "Runtime error: " << e.what() << std::endl;
        return -1;
    }

    SDL_Quit();
    return 0;
}
