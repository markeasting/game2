#include <SDL2/SDL.h>
#include <SDL_scancode.h>

#include "common/ref.h"
#include "core/Window.h"
#include "util/VectorMath.h"

#include "gfx/Renderer.h"
#include "gfx/CubeMapTexture.h"

#include "gfx/fxpass/BloomPass.h"
#include "gfx/fxpass/FinalCompositePass.h"
#include "gfx/fxpass/FogPass.h"
#include "gfx/fxpass/SmearPass.h"

#include "geom/TetrahedronGeometry.h"
#include "geom/PlaneGeometry.h"
#include "geom/BoxGeometry.h"

#include "phys/PhysicsHandler.h"
#include "phys/RigidBody.h"

#include "gameobject/GameObject.h"
#include "component/CameraController.h"

// ImGui
#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_opengl3.h"

// Game-specific 
#include "Car.h"

void _lerp(float& value, float target, float factor = 0.98) {
    value = std::lerp(value, target, factor);
}

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
            .useRenderpass = true,
            .renderPasses = {
                // ref<FogPass>(),
                ref<BloomPass>(),
                // ref<SmearPass>(),
                // ref<FinalCompositePass>(),
            }
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
        auto& player = car;
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
                        // camController->m_autoRotate = !camController->m_autoRotate;
                        // camController->m_enableFreeCam = !camController->m_enableFreeCam;
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
            if (state[SDL_SCANCODE_ESCAPE])
                camController->m_enableFreeCam = false;
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

            /* Player controls */
            if (!camController->m_enableFreeCam && !camController->m_autoRotate) {
                // camController->m_lookAtPos = player.m_body->localToWorld(player.m_camLookPos);
                // camController->setPosition(player.m_body->localToWorld(player.m_camPos));
                camController->lookAt(player.m_camLookPos);
                cameraObject->m_transform->setPosition(player.m_camPos);

                // if (Gamepad::m_active) {
                //     player.applySteering(Gamepad::m_axes[0]);
                //     player.applyThrottle(-Gamepad::m_axes[3]);
                // }

                if (state[SDL_SCANCODE_W]) {
                    _lerp(player.m_throttle, 1.0f, 0.15f);
                } else if (state[SDL_SCANCODE_S]) {
                    _lerp(player.m_throttle, -1.0f, 0.15f);
                } else {
                    _lerp(player.m_throttle, 0.0f, 0.15f);
                }

                if (state[SDL_SCANCODE_A]) {
                    _lerp(player.m_steering, -1.0f, (clamp(player.getLinearVelocity() / 50.0f, 0.09f, 0.15f)));
                } else if (state[SDL_SCANCODE_D]) {
                    _lerp(player.m_steering, 1.0f, (clamp(player.getLinearVelocity() / 50.0f, 0.09f, 0.15f)));
                } else {
                    _lerp(player.m_steering, 0.0f, (clamp(player.getLinearVelocity() / 50.0f, 0.09f, 0.15f)));
                }

                player.m_handbrake = state[SDL_SCANCODE_B] == true; // || Gamepad::isButtonPressed(SDL_CONTROLLER_BUTTON_B);

                // if (Keyboard::a) 
                //     player.applySteering(-1.0f);
                // if (Keyboard::d) 
                //     player.applySteering(1.0f);
            }


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

            ImGui::Separator();

            ImGui::Checkbox("Auto-rotate", &camController->m_autoRotate);
            ImGui::Checkbox("Free cam", &camController->m_enableFreeCam);
            ImGui::SliderFloat("Camera speed", &camController->m_speed, 0.1f, 10.0f);
            
            ImGui::Separator();

            auto grip = player.m_wheels[0]->m_grip;
            if (ImGui::SliderFloat("Grip", &grip, 0.0f, 2000.0f)) {
                car.setGrip(grip);
            }

            auto stiffness = player.m_wheels[0]->m_stiffness;
            if (ImGui::SliderFloat("Stiffness", &stiffness, 1000.0f, 20000.0f)) {
                car.setSpringStiffness(stiffness);
            }

            auto damping = player.m_wheels[0]->m_damping;
            if (ImGui::SliderFloat("Damping", &damping, 0.0f, 1000.0f)) {
                car.setDamping(damping);
            }

            ImGui::Separator();
            auto colorUni = colorMaterial2.getUniform<vec4>("u_color");
            ImGui::ColorEdit4("Color", &colorUni->m_value[0], ImGuiColorEditFlags_HDR);

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
