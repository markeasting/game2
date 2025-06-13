#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <iostream>

int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) < 0) {
        throw std::runtime_error(
            std::string("SDL could not be initialized: ") + SDL_GetError()
        );
    }

    SDL_Window *window = SDL_CreateWindow(
        "MOI",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        500, 500, 
        SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI // | SDL_WINDOW_BORDERLESS
    );

    if (!window) {
        throw std::runtime_error(
            std::string("Window could not be created: ") + SDL_GetError()
        );
    }

    // SDL_SetWindowFullscreen(m_window, SDL_WINDOW_FULLSCREEN_DESKTOP);

    /* OpenGL context */
    int context_flags = SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG;
    
    #ifdef DEBUG_BUILD
        context_flags |= SDL_GL_CONTEXT_DEBUG_FLAG;
    #endif

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, context_flags);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    #ifdef __APPLE__
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    #else
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    #endif

    SDL_GLContext gl_context = SDL_GL_CreateContext(window);

    // SDL_GetWindowSize
    int m_frameBufferWidth = 0;
    int m_frameBufferHeight = 0;
    SDL_GL_GetDrawableSize(window, &m_frameBufferWidth, &m_frameBufferHeight);

    if (!gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress)) {
        throw std::runtime_error("Failed to initialize OpenGL context");
    }

    SDL_GL_SetSwapInterval(0);

    printf("[OpenGL] Device:  %s\n", glGetString(GL_RENDERER));
    printf("[OpenGL] Version: %s\n", glGetString(GL_VERSION));
    printf("[OpenGL] Window:  %d x %d\n", m_frameBufferWidth, m_frameBufferHeight);

    /* Event loop */
    bool quit = false;

    SDL_Event e;

    while (!quit)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_KEYDOWN)
            {
                std::cout << "Key pressed: " << e.key.keysym.sym << std::endl;
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                std::cout << "Mouse button pressed: " << e.button.button << std::endl;
            }
            else if (e.type == SDL_WINDOWEVENT)
            {
                if (e.window.event == SDL_WINDOWEVENT_CLOSE)
                {
                    quit = true;
                }
            }

            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
        }
    }

    return 0;
}
