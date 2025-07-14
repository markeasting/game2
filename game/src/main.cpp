#include <SDL2/SDL.h>

#include "Car.h"
#include "common/ref.h"
#include "core/Window.h"
#include "util/VectorMath.h"

#include "gfx/Renderer.h"
#include "gfx/CubeMapTexture.h"

#include "geom/TetrahedronGeometry.h"
#include "geom/PlaneGeometry.h"
#include "geom/BoxGeometry.h"

#include "phys/PhysicsHandler.h"
#include "phys/RigidBody.h"

#include "gameobject/GameObject.h"
#include "component/CameraController.h"

#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_opengl3.h"

int main() {

    /* Event loop */
    bool quit = false;
    SDL_Event e;

    /* Timer */
    float time = 0.0f;
    float prevTime = 0.0f;
    float dt = 0.0f;

    try {
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) < 0) {
            throw std::runtime_error(
                std::string("SDL could not be initialized: ") + SDL_GetError()
            );
        }
        
        Window window({
            .windowTitle = "MOIII",
            .windowWidth = 768,
            .windowHeight = 768,
            .fullscreen = false,
            .vsync = true
        });

        /* Init GUI */
        ImGui::CreateContext();
        // ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGui::StyleColorsDark();
        ImGui_ImplSDL2_InitForOpenGL(window.getSDLWindow(), window.getGLContext());
        ImGui_ImplOpenGL3_Init("#version 330 core");

        /* Initialize renderer */
        Renderer renderer = Renderer({
            .wireframe = false,
            .useRenderpass = true
        });

        renderer.setSize(
            window.getFrameBufferWidth(), // @todo make part of RendererConfig
            window.getFrameBufferHeight()
        );

        /* Initialize physics */
        PhysicsHandler phys;
        phys.init();

        /* Scene / hierarchy */
        std::vector<Ref<GameObject>> gameObjects;
        
        auto cameraObject = ref<GameObject>("Camera", std::vector<Ref<Component>>{
            ref<Camera>(),
            ref<CameraController>()
        });

        gameObjects.push_back(cameraObject);
        
        cameraObject->m_transform->setPosition({ 0.0f, 2.0f, 5.0f });

        auto camera = cameraObject->getComponent<Camera>();
        auto camController = cameraObject->getComponent<CameraController>();
        
        camController->m_autoRotate = true;

        camera->setSize(
            window.getFrameBufferWidth(),
            window.getFrameBufferHeight()
        );

        auto colorShader = ref<Shader>("Color");

        auto colorMaterial = Material(colorShader, {
            { "u_color", uniform(vec4(0.0f, 1.0f, 0.2f, 1.0f)) },
        });
        
        auto colorMaterial2 = Material(colorShader, {
            { "u_color", uniform(vec4(10.0f, 5.0f, 0.8f, 1.0f)) },
        });

        auto textureMaterial = Material(ref<Shader>("Basic.vert", "BasicTextured.frag"));
        textureMaterial.assignTexture("assets/texture/default.jpg", "texture1");
        textureMaterial.assignTexture("assets/texture/uv_test.jpg", "texture2");

        auto tetra = ref<GameObject>("Tetrahedron", std::vector<Ref<Component>>{
            ref<Mesh>(ref<TetrahedronGeometry>(1.0f), colorMaterial)
        });
        
        tetra->m_transform->setPosition({ 0.0f, 1.0f, -2.0f });

        gameObjects.push_back(tetra);

        auto box = ref<GameObject>("Box", std::vector<Ref<Component>>{
            ref<Mesh>(BoxGeometry(), textureMaterial)
        });
        box->m_transform->setPosition({ 0.0f, 1.0f, -1.5f });
        box->m_transform->setScale(vec3(0.5f, 0.5f, 0.5f));

        tetra->m_transform->add(box->m_transform);

        gameObjects.push_back(box);

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

        auto skybox = ref<GameObject>("Skybox", std::vector<Ref<Component>>{
            ref<Mesh>(BoxGeometry(1.0f, true), skyMaterial)
        });

        gameObjects.push_back(skybox);

        /* Physics bodies */
        Ref<RigidBody> player = nullptr;

        const float size = 0.5f;
        for (size_t i = 0; i < 4; i++) {

            auto box = ref<GameObject>("Box", std::vector<Ref<Component>>{
                ref<Mesh>(BoxGeometry(size), textureMaterial),
                ref<RigidBody>(
                    ref<BoxCollider>(size)
                )
            });

            gameObjects.push_back(box);

            // auto body = box->addComponent<RigidBody>(ref<BoxCollider>(size));
            auto body = box->getComponent<RigidBody>();

            body->setPosition({ 0.0f, size + i * size * 1.001f, 0.0f });
            body->setBox(vec3(size, size, size), 150.0f);

            if (i == 0) {
                box->getComponent<Mesh>()->setMaterial(colorMaterial2);
                player = body;
            }
        }

        auto floor = ref<GameObject>("Floor", std::vector<Ref<Component>>{
            ref<Mesh>(PlaneGeometry(300.0f, 300.0f), textureMaterial),
            ref<RigidBody>()
        });
        auto floorBody = floor->getComponent<RigidBody>();

        floorBody->setPosition({ 0.0f, 0.0f, 0.0f });
        floorBody->setRotation(QuatFromTwoVectors(vec3(0.0f, 1.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f)));
        floorBody->makeStatic();

        gameObjects.push_back(floor);

        /* Car */
        auto car = Car(phys);
        auto carBody = car.m_body->getComponent<RigidBody>();
        // phys.add(car.m_body->getComponent<RigidBody>());

        gameObjects.push_back(car.m_body);
        gameObjects.push_back(car.m_bodyShadow);
        for (auto& wheel : car.m_wheels) {
            gameObjects.push_back(wheel);
        }
        
        /* get all mesh components from all game objects */
        std::vector<Ref<Mesh>> meshes;
        for (const auto& obj : gameObjects) {
            
            if (auto mesh = obj->tryGetComponent<Mesh>()) {
                meshes.push_back(mesh);
            }

            if (auto body = obj->tryGetComponent<RigidBody>()) {
                phys.add(body);
            }
        }

        /* Main loop */
        while (!quit) {

            /* Update timer */
            prevTime = time;
            time = SDL_GetTicks() / 1000.0f;
            dt = time - prevTime;
            
            /* Event polling */
            while (SDL_PollEvent(&e)) {

                ImGui_ImplSDL2_ProcessEvent(&e);
                
                if (e.type == SDL_KEYDOWN) {
                    std::cout << "Key pressed: " << e.key.keysym.sym << std::endl;

                    if (e.key.keysym.sym == SDLK_p) {
                        Shader::refreshAll();
                    }
                
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

            /* Game loop */
            const Uint8* state = SDL_GetKeyboardState(NULL);

            if (state[SDL_SCANCODE_SPACE])
                carBody->applyForce(vec3(0.0f, 10000.0f, 0.0f), carBody->pose.p);
            if (state[SDL_SCANCODE_J])
                carBody->applyForce(vec3(-250.0f, 0.0f, 0.0f), carBody->pose.p);
            if (state[SDL_SCANCODE_L])
                carBody->applyForce(vec3(250.0f, 0.0f, 0.0f), carBody->pose.p);
            if (state[SDL_SCANCODE_I])
                carBody->applyForce(vec3(0.0f, 0.0f, -250.0f), carBody->pose.p);
            if (state[SDL_SCANCODE_K])
                carBody->applyForce(vec3(0.0f, 0.0f, 250.0f), carBody->pose.p);

            float osc = sin(time * 1.5f) / 2.0f + 0.5f;
            colorMaterial.setUniform("u_color", vec4(0.0f, osc, 0.8f, 1.0f));

            tetra->m_transform->setPosition({ 0.0f, 1.0f + osc, -2.0f });
            tetra->m_transform->setRotation(vec3(0.0f, time * 50.0f, 0.0f));

            /* Core update */
            phys.update(dt, [&](float h) {
                car.update(h, phys);
            });

            for (auto& obj : gameObjects) {
                obj->update(time, dt);
            }

            renderer.draw(meshes, camera);
            // renderer.clear(); // used internally in Renderer::render()

            /* GUI */
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplSDL2_NewFrame();
            ImGui::NewFrame();
            
            ImGui::Begin("GUI");
            ImGui::BulletText("time: %.2fs", time);
            ImGui::BulletText("fps: %.1f", ImGui::GetIO().Framerate);
            ImGui::BulletText("dt: %.3f", dt);
            
            ImGui::Separator();

            if (ImGui::Button("Refresh Shaders")) {
                Shader::refreshAll();
            }

            ImGui::End();
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            /* Swap buffers / main loop end */
            window.swapBuffers();
        }
    } catch (const std::exception &e) {
        std::cerr << "Runtime error: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}

