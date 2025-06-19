#include <SDL2/SDL.h>
#include <iostream>
#include <vector>

#include "Window.h"
#include "gfx/Renderer.h"
#include "geom/TetrahedronGeometry.h"
#include "geom/BoxGeometry.h"
#include "geom/ArrowGeometry.h"
#include "gfx/CubeMapTexture.h"

int main() {

    try {
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) < 0) {
            throw std::runtime_error(
                std::string("SDL could not be initialized: ") + SDL_GetError()
            );
        }

        Window window({
            .windowTitle = "MOIII",
            .windowWidth = 1500,
            .windowHeight = 1500,
            .fullscreen = false,
            .vsync = true
        });

        Renderer renderer = Renderer({
            .wireframe = false,
            .useRenderpass = true
        });

        std::cout << window.getFrameBufferWidth() << "x" 
                  << window.getFrameBufferHeight() << std::endl;

        renderer.setSize(
            window.getFrameBufferWidth(), // @todo make part of RendererConfig
            window.getFrameBufferHeight()
        );
        
        auto camera = ref<Camera>();

        // camera->m_autoRotate = true;
        camera->setSize(
            window.getFrameBufferWidth(),
            window.getFrameBufferHeight()
        );
        
        auto colorMaterial = Material("Color", {
            uniform("u_color", vec4(1.0f, 0.0f, 0.8f, 1.0f)),
        });

        auto mesh = ref<Mesh>(TetrahedronGeometry(0.2f), colorMaterial);
        mesh->setPosition({ 0.0f, 0.0f, 0.0f });

        auto line = ref<Mesh>(ArrowGeometry(), colorMaterial);

        Material skyMaterial = Material("SkyBox", {
            uniform("u_color", vec4(0.0f, 0.0f, 0.8f, 1.0f)),
        });
        Ref<CubeMapTexture> skyTexture = ref<CubeMapTexture>();
        skyTexture->loadCubemap({
            "assets/texture/skybox/right.jpg",
            "assets/texture/skybox/left.jpg",
            "assets/texture/skybox/bottom.jpg",
            "assets/texture/skybox/top.jpg",
            "assets/texture/skybox/front.jpg",
            "assets/texture/skybox/back.jpg"
        });
        skyMaterial.assignTexture(skyTexture, "texture1");
        auto skybox = ref<Mesh>(BoxGeometry(1.0f, true), skyMaterial);

        auto meshes = std::vector<Ref<Mesh>>();
        meshes.push_back(mesh);
        meshes.push_back(skybox);
        // meshes.push_back(line);
        // meshes.push_back(scene->m_tetra);
        // meshes.push_back(scene->m_skybox);

        /* Event loop */
        bool quit = false;
        SDL_Event e;

        while (!quit) {
        
            float time = SDL_GetTicks() / 1000.0f;
            
            // std::cout << "Time: " << time << " seconds" << std::endl;
            
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

                // switch (e.type) {
                //     case SDL_KEYUP:
                //     case SDL_KEYDOWN:
                //         scene->onKey(e.key);
                //         break;

                //     case SDL_MOUSEBUTTONUP:
                //     // case SDL_MOUSEBUTTONDOWN:
                //         scene->onClick(e.button);
                //         break;

                //     case SDL_CONTROLLERBUTTONUP:
                //         scene->onGamepadButton(e.cbutton);
                //         break;
                
                //     default:
                //         break;
                // }
            }

            // scene->update(time, 0.016f); 
            camera->update(time);

            // renderer.draw(scene, camera);
            renderer.draw(meshes, camera);

            // renderer.clear(); // used internally in Renderer::render()
            window.swapBuffers();
        }
    } catch (const std::exception &e) {
        std::cerr << "Runtime error: " << e.what() << std::endl;
        return -1;
    }

    // auto container = Container::instance();
    
    // Engine::CoreBundle::configure({
    //     .window {
    //         .windowTitle = "MOI",
    //         .windowWidth = 640,
    //         .windowHeight = 480,
    //         .fullscreen = false,
    //         .vsync = false,
    //     },
    //     .renderer {
    //         .wireframe = false
    //     },
    // });

    // Engine::SceneManagerBundle::configure();

    // /* Game class */
    // container->singleton<Game, Timer, Window, Renderer, SceneManager>();

    // const auto game = container->resolve<Game>();

    // /* @TODO should just have some generic 'race' scene where the map is loaded dynamically */
    // // game->m_sceneManager.add("menu", ref<Menu>());
    // game->m_sceneManager->add("playground", ref<PlayGround>());
    // game->m_sceneManager->switchTo("playground");

    // game->initialize();
    
    // while (game->isRunning()) {
    //     game->update();
    // }

    // game->quit();

    return 0;
}

