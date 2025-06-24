#include <SDL2/SDL.h>
#include <iostream>
#include <vector>

#include "core/CameraController.h"
#include "core/Window.h"
#include "gfx/Renderer.h"
#include "geom/TetrahedronGeometry.h"
#include "geom/PlaneGeometry.h"
#include "geom/BoxGeometry.h"
#include "gfx/CubeMapTexture.h"

#include "phys/PhysicsHandler.h"
#include "phys/RigidBody.h"
#include "util/VectorMath.h"

// #include "Car.h"

int main() {

    try {
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) < 0) {
            throw std::runtime_error(
                std::string("SDL could not be initialized: ") + SDL_GetError()
            );
        }

        Window window({
            .windowTitle = "MOIII",
            .windowWidth = 500,
            .windowHeight = 500,
            .fullscreen = false,
            .vsync = true
        });

        Renderer renderer = Renderer({
            .wireframe = false,
            .useRenderpass = true
        });

        renderer.setSize(
            window.getFrameBufferWidth(), // @todo make part of RendererConfig
            window.getFrameBufferHeight()
        );
        
        auto camera = ref<Camera>();
        
        auto camController = ref<CameraController>(camera);
        camController->setPosition({ 0.0f, 2.0f, 5.0f });

        // camera->m_autoRotate = true;
        camera->setSize(
            window.getFrameBufferWidth(),
            window.getFrameBufferHeight()
        );

        auto colorShader = ref<Shader>("Color");

        auto colorMaterial = Material(colorShader, {
            { "u_color", uniform(vec4(0.0f, 1.0f, 0.2f, 1.0f)) },
        });
        
        auto colorMaterial2 = Material(colorShader, {
            { "u_color", uniform(vec4(1.0f, 0.0f, 0.8f, 1.0f)) },
        });

        auto textureMaterial = Material(ref<Shader>("Basic.vert", "BasicTextured.frag"));
        textureMaterial.assignTexture("assets/texture/default.jpg", "texture1");
        textureMaterial.assignTexture("assets/texture/uv_test.jpg", "texture2");

        auto tetra = ref<Mesh>(TetrahedronGeometry(1.0f), colorMaterial);
        tetra->setPosition({ 0.0f, 1.0f, -2.0f });

        auto box = ref<Mesh>(BoxGeometry(), textureMaterial);
        box->setPosition({ 0.0f, 2.0f, -3.0f });

        Material skyMaterial = Material(ref<Shader>("SkyBox"));
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

        auto meshes = std::vector<Ref<Mesh>>({
            tetra,
            box,
            skybox
        });

        PhysicsHandler phys;
        phys.init();

        Ref<RigidBody> player = nullptr;

        const float size = 0.5f;
        for (size_t i = 0; i < 4; i++) {
            auto box = ref<RigidBody>(
                ref<BoxCollider>(size), 
                ref<Mesh>(BoxGeometry(size), textureMaterial)
            );
            box->setPosition({ 0.0f, size + i * size * 1.001f, 0.0f });
            box->setBox(vec3(size, size, size), 150.0f);
            // box->canSleep = false;
            meshes.push_back(box->mesh);
            phys.add(box);

            if (i == 0)
                player = box;
        }

        auto floorMesh = ref<Mesh>(PlaneGeometry(3000.0f, 3000.0f), colorMaterial2);
        auto floor = ref<RigidBody>(floorMesh);
        // floor->staticFriction = 0.8f;
        // floor->dynamicFriction = 0.8f;

        floor->setPosition({ 0.0f, 0.0f, 0.0f });
        floor->setRotation(QuatFromTwoVectors(vec3(0.0f, 1.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f)));

        floor->makeStatic();
        phys.add(floor);
        meshes.push_back(floorMesh);

        // auto car = ref<Car>(phys);
        // phys.add(car->m_body);
        // meshes.push_back(car->m_body->mesh);
        // meshes.push_back(car->m_bodyShadow);
        // for (auto& wheel : car->m_wheels) {
        //     meshes.push_back(wheel.m_mesh);
        //     meshes.push_back(wheel.m_origin);
        //     meshes.push_back(wheel.m_line);
        //     meshes.push_back(wheel.m_line2);
        // }

        /* Event loop */
        bool quit = false;
        SDL_Event e;

        /* Timer */
        float time = 0.0f;
        float prevTime = 0.0f;
        float dt = 0.0f;

        while (!quit) {
        
            prevTime = time;
            time = SDL_GetTicks() / 1000.0f;
            dt = time - prevTime;
            
            while (SDL_PollEvent(&e)) {
                
                if (e.type == SDL_KEYDOWN) {
                    std::cout << "Key pressed: " << e.key.keysym.sym << std::endl;
                
                } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                    std::cout << "Mouse button pressed: " << e.button.button << std::endl;

                    if (e.button.button == SDL_BUTTON_LEFT) {
                        camController->m_autoRotate = !camController->m_autoRotate;
                    }
                
                } else if (e.type == SDL_WINDOWEVENT) {
                    if (e.window.event == SDL_WINDOWEVENT_RESIZED) {
                        // const int width = e.window.data1; // Not hidpi compatible
                        // const int height = e.window.data2; // Not hidpi compatible
                        auto [w, h] = window.handleResize();
                        renderer.setSize(w, h);
                        camera->setSize(w, h);
                    }
                    
                    if (e.window.event == SDL_WINDOWEVENT_CLOSE) {
                        quit = true;
                    }
                }

                if (e.type == SDL_QUIT) {
                    quit = true;
                }
            }

            const Uint8* state = SDL_GetKeyboardState(NULL);

            if (state[SDL_SCANCODE_SPACE])
                player->applyForce(vec3(0.0f, 250.0f, 0.0f), player->pose.p);
            if (state[SDL_SCANCODE_J])
                player->applyForce(vec3(-250.0f, 0.0f, 0.0f), player->pose.p);
            if (state[SDL_SCANCODE_L])
                player->applyForce(vec3(250.0f, 0.0f, 0.0f), player->pose.p);
            if (state[SDL_SCANCODE_I])
                player->applyForce(vec3(0.0f, 0.0f, -250.0f), player->pose.p);
            if (state[SDL_SCANCODE_K])
                player->applyForce(vec3(0.0f, 0.0f, 250.0f), player->pose.p);

            float osc = sin(time * 1.5f) / 2.0f + 0.5f;
            colorMaterial.setUniform("u_color", vec4(0.0f, osc, 0.8f, 1.0f));

            phys.update(dt, [&](float dt) {});
            
            camController->update(time);
            camera->update();

            // renderer.draw(scene, camera);
            renderer.draw(meshes, camera);

            // renderer.clear(); // used internally in Renderer::render()
            window.swapBuffers();
        }
    } catch (const std::exception &e) {
        std::cerr << "Runtime error: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}

