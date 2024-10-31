#include "Application.hpp"
#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <SOIL/SOIL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <sol/sol.hpp>
#include <iostream>

#include <Logger/Logger.hpp>
#include <Rendering/Essentials/ShaderLoader.hpp>
#include <Rendering/Essentials/TextureLoader.hpp>
#include <Rendering/Core/Camera2D.hpp>
#include <Rendering/Core/Renderer.hpp>
#include <Rendering/Essentials/Vertex.hpp>
#include <Core/ECS/Entity.hpp>
#include <Core/ECS/Components/TransformComponent.hpp>
#include <Core/ECS/Components/SpriteComponent.hpp>
#include <Core/ECS/Components/PhysicsComponent.hpp>
#include <Core/ECS/Components/BoxColliderComponent.hpp>
#include <Core/ECS/Components/CircleColliderComponent.hpp>
#include <Core/ECS/Components/Identification.hpp>
#include <Core/Resources/AssetManager.hpp>

#include <Core/Systems/ScriptingSystem.hpp>
#include <Core/Systems/RenderSystem.hpp>
#include <Core/Systems/AnimationSystem.hpp>
#include <Core/Systems/PhysicsSystem.hpp>
#include <Core/Systems/RenderShapeSystem.hpp>
#include <Core/Systems/RenderUISystem.hpp>

#include <Core/Scripting/InputManager.hpp>
#include <Windowing/Inputs/Keyboard.hpp>
#include <Windowing/Inputs/Mouse.hpp>
#include <Sounds/MusicPlayer/MusicPlayer.hpp>
#include <Sounds/SoundPlayer/SoundFxPlayer.hpp>
#include <Physics/ContactListener.hpp>

// IMGUI TESTING ======================
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>
#include <SDL_opengl.h>
// ===================================


namespace ENGINE_EDITOR
{

    Application::Application()
		: m_pWindow{nullptr}, m_pRegistry{nullptr}, m_Event{}, m_bIsRunning{true}
	{ }

    Application& Application::GetInstance()
    {
        // "static" allows to do this only one time 
        static Application app{};
        return app;
    }


    bool Application::Initialize()
	{
        ENGINE_INIT_LOGS(true, true);

        if ( SDL_Init( SDL_INIT_EVERYTHING ) != 0 )
        {
            std::string error = SDL_GetError();
            ENGINE_ERROR("Failed to initialize SDL: {0}", error);
            return false;
        }
        
        if ( SDL_GL_LoadLibrary( NULL ) != 0 )
        {
            std::string error = SDL_GetError();
            ENGINE_ERROR("Failed to Open GL Library: {0}", error);
            return false;
        }

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1); //1=hardware accelleration, 0=software rendering

        //SDL_DisplayMode displayMode;
	    //SDL_GetCurrentDisplayMode( 0, &displayMode );

        m_pWindow = std::make_unique<ENGINE_WINDOWING::Window>(
            "Test window", 
            640, 480, 
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
            true, 
            SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_MOUSE_CAPTURE //| SDL_WINDOW_MAXIMIZED 
        );
    
        if ( !m_pWindow->GetWindow() )
        {
            ENGINE_ERROR("Failed to create the window!"); 
            return false;
        }

        m_pWindow->SetGLContext(SDL_GL_CreateContext(m_pWindow->GetWindow().get()));
        
        if ( gladLoadGLLoader( SDL_GL_GetProcAddress ) == 0 )
        {
            ENGINE_ERROR("Failed to Initialize Glad");
            return false;
        }
        
        if (!m_pWindow->GetGLContext())
        {
            std::string error = SDL_GetError();
            ENGINE_ERROR("Failed to create OpenGL Context!", error);
            return false;
        }

        SDL_GL_MakeCurrent(m_pWindow->GetWindow().get(), m_pWindow->GetGLContext());
        SDL_GL_SetSwapInterval( 1 );

        ////////////////////////////////////////////////////////////////////////////////////////////////////////

        // Renderer
        auto renderer = std::make_shared<ENGINE_RENDERING::Renderer>();

        // Enable Alpha Blending
        renderer->SetCapability(ENGINE_RENDERING::Renderer::GLCapability::BLEND, true);
		renderer->SetBlendCapability(
			ENGINE_RENDERING::Renderer::BlendingFactors::SRC_ALPHA,
			ENGINE_RENDERING::Renderer::BlendingFactors::ONE_MINUS_SRC_ALPHA
		);
        //glEnable(GL_BLEND);
        //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        GetOpenGLVersionInfo();
    
        auto assetManager = std::make_shared<ENGINE_RESOURCES::AssetManager>();
        if(!assetManager)
        {
            ENGINE_ERROR("Failed to create the Asset manager");
            return false;
        }
        // Texture //////////////////////
        /*if(!assetManager->AddTexture("16map", "./assets/textures/16map.png", true))
        {
            ENGINE_ERROR("Failed to Create and Add Texture");
            return false;
        }
        if(!assetManager->AddTexture("robot", "./assets/textures/robotSprite.png", true))
        {
            ENGINE_ERROR("Failed to Create and Add Texture");
            return false;
        }*/

        assetManager->AddTexture("ball", "assets/textures/ball_13.png");
  
        m_pRegistry = std::make_unique<ENGINE_CORE::ECS::Registry>();

        if(!m_pRegistry->AddToContext<std::shared_ptr<ENGINE_RENDERING::Renderer>>(renderer));
        {
            ENGINE_ERROR("Failed to add the Renderer to Registry Context");
            //return false;
        }

        // Lua state //////////////////////
        auto lua = std::make_shared<sol::state>();

        if(!lua)
        {
            ENGINE_ERROR("Failed to create the Lua State");
            return false;
        }
        lua->open_libraries(sol::lib::base, sol::lib::math, sol::lib::os, sol::lib::table, sol::lib::io, sol::lib::string);

        m_pRegistry->AddToContext<std::shared_ptr<sol::state>>(lua);
        if(!m_pRegistry->AddToContext<std::shared_ptr<sol::state>>(lua));
        {
            ENGINE_ERROR("Failed to add the sol::state to Registry Context");
            //return false;
        }
        // Script System
        auto scriptSystem = std::make_shared<ENGINE_CORE::Systems::ScriptingSystem>(*m_pRegistry);
        if(!scriptSystem)
        {
            ENGINE_ERROR("Failed to create the Scripting System");
            return false;   
        }
        if(!m_pRegistry->AddToContext<std::shared_ptr<ENGINE_CORE::Systems::ScriptingSystem>>(scriptSystem))
        {
            ENGINE_ERROR("Failed to add the ScriptingSystem to Registry Context");
            return false;
        }
        // Render System
        auto renderSystem = std::make_shared<ENGINE_CORE::Systems::RenderSystem>(*m_pRegistry);
        if(!renderSystem)
        {
            ENGINE_ERROR("Failed to create the Render System");
            return false;   
        }
        if(!m_pRegistry->AddToContext<std::shared_ptr<ENGINE_CORE::Systems::RenderSystem>>(renderSystem))
        {
            ENGINE_ERROR("Failed to add the RenderSystem to Registry Context");
            return false;
        }
        auto renderUISystem = std::make_shared<ENGINE_CORE::Systems::RenderUISystem>(*m_pRegistry);
        if(!renderUISystem)
        {
            ENGINE_ERROR("Failed to create the RenderUI System");
            return false;   
        }
        if(!m_pRegistry->AddToContext<std::shared_ptr<ENGINE_CORE::Systems::RenderUISystem>>(renderUISystem))
        {
            ENGINE_ERROR("Failed to add the RenderUISystem to Registry Context");
            return false;
        }
        auto renderShapeSystem = std::make_shared<ENGINE_CORE::Systems::RenderShapeSystem>(*m_pRegistry);
        if(!renderShapeSystem)
        {
            ENGINE_ERROR("Failed to create the RenderShape System");
            return false;   
        }
        if(!m_pRegistry->AddToContext<std::shared_ptr<ENGINE_CORE::Systems::RenderShapeSystem>>(renderShapeSystem))
        {
            ENGINE_ERROR("Failed to add the RenderShapeSystem to Registry Context");
            return false;
        }
        // Animation System
        auto animationSystem = std::make_shared<ENGINE_CORE::Systems::AnimationSystem>(*m_pRegistry);
        if(!animationSystem)
        {
            ENGINE_ERROR("Failed to create the Animation System");
            return false;   
        }
        if(!m_pRegistry->AddToContext<std::shared_ptr<ENGINE_CORE::Systems::AnimationSystem>>(animationSystem))
        {
            ENGINE_ERROR("Failed to add the AnimationSystem to Registry Context");
            return false;
        }

        // Sound ///////////////////////
        auto musicPlayer = std::make_shared<ENGINE_SOUNDS::MusicPlayer>();
        if(!musicPlayer)
        {
            ENGINE_ERROR("Failed to create the Music Player");
            return false;   
        }
        if(!m_pRegistry->AddToContext<std::shared_ptr<ENGINE_SOUNDS::MusicPlayer>>(musicPlayer))
        {
            ENGINE_ERROR("Failed to add the Music Player to Registry Context");
            return false;
        }

        auto soundPlayer = std::make_shared<ENGINE_SOUNDS::SoundFxPlayer>();
        if(!soundPlayer)
        {
            ENGINE_ERROR("Failed to create the SoundFX Player");
            return false;   
        }
        if(!m_pRegistry->AddToContext<std::shared_ptr<ENGINE_SOUNDS::SoundFxPlayer>>(soundPlayer))
        {
            ENGINE_ERROR("Failed to add the SoundFX Player to Registry Context");
            return false;
        }

        // Camera //////////////////////
        auto camera = std::make_shared<ENGINE_RENDERING::Camera2D>();

        if(!m_pRegistry->AddToContext<std::shared_ptr<ENGINE_RESOURCES::AssetManager>>(assetManager))
        {
            ENGINE_ERROR("Failed to add the Asset Manager to Registry Context");
            return false;
        }

        if(!m_pRegistry->AddToContext<std::shared_ptr<ENGINE_RENDERING::Camera2D>>(camera))
        {
            ENGINE_ERROR("Failed to add the Camera to Registry Context");
            return false;
        }

        // Physics /////////////////////
        ENGINE_PHYSICS::PhysicsWorld pPhysicsWorld = std::make_shared<b2World>(b2Vec2{0.f, 9.8f});
        if(!m_pRegistry->AddToContext<ENGINE_PHYSICS::PhysicsWorld>(pPhysicsWorld))
        {
            ENGINE_ERROR("Failed to add the Physics World to Registry Context");
            return false;
        }

        auto pPhysicsSystem = std::make_shared<ENGINE_CORE::Systems::PhysicsSystem>(*m_pRegistry);
        if(!m_pRegistry->AddToContext<std::shared_ptr<ENGINE_CORE::Systems::PhysicsSystem>>(pPhysicsSystem))
        {
            ENGINE_ERROR("Failed to add the Physics System to Registry Context");
            return false;
        }

        auto pContactListener = std::make_shared<ENGINE_PHYSICS::ContactListener>();
		if (!m_pRegistry->AddToContext<std::shared_ptr<ENGINE_PHYSICS::ContactListener>>(pContactListener))
		{
			ENGINE_ERROR("Failed to add the contact listener to the registry context!");
			return false;
		}

		pPhysicsWorld->SetContactListener(pContactListener.get());
        ////////////////////////////////////////////////////////////////////////////////////////////////////////

        if(!InitImGui())
        {
            ENGINE_ERROR("Failed to initialize ImGui");
            return false;
        }

        if(!LoadShaders())
        {
            ENGINE_ERROR("Failed to load the Shaders");
            return false;
        }

        ENGINE_CORE::Systems::ScriptingSystem::RegisterLuaBinding(*lua, *m_pRegistry);
        ENGINE_CORE::Systems::ScriptingSystem::RegisterLuaFunctions(*lua, *m_pRegistry);

        if(!scriptSystem->LoadMainScript(*lua))
        {
            ENGINE_ERROR("Failed to load the Main Lua Script");
            return false; 
        }

        renderer->SetLineWidth(4.f);

        if(!assetManager->AddFont("pixel", "./assets/fonts/retro_pixel.TTF"))
        {
            ENGINE_ERROR("Failed to load Pixel Font");
            return false;
        }

        /*auto pTexture = assetManager->GetTexture("ball");
        using namespace ENGINE_CORE::ECS;
        auto& reg = m_pRegistry->GetRegistry();

        auto entity1 = reg.create();
        auto& transform1 = reg.emplace<TransformComponent>(
            entity1,
            TransformComponent{
                .position = glm::vec2{420.f, 0.f},
                .scale = glm::vec2{1.f}
            }
        );
        auto circle1 = reg.emplace<CircleColliderComponent>(
            entity1,
            CircleColliderComponent{
                .radius = 22.f,
            }
        );
        auto& physics1 = reg.emplace<PhysicsComponent>(
            entity1,
            PhysicsComponent{
                PhysicsAttributes{
                    .eType = RigidBodyType::DYNAMIC,
                    .density = 100.f,
                    .friction = 0.f,
                    .restituton = 0.4f,
                    .radius = circle1.radius * PIXELS_TO_METERS,
                    .gravityScale = 5.f,
                    .position = transform1.position,
                    .scale = transform1.scale,
                    .bCircle = true,
                    .bFixedRotation = false
                }
            }
        );
        physics1.Init(pPhysicsWorld ,640, 480);
        
        auto& sprite = reg.emplace<SpriteComponent>(
            entity1,
            SpriteComponent{
                .width = 42.f,
                .height = 42.f,
                .start_x = 0,
                .start_y = 0,
                .texture_name = "ball"
            }
        );
        sprite.generate_uvs(42, 42);

        auto entity2 = reg.create();
        auto& transform2 = reg.emplace<TransformComponent>(
            entity2,
            TransformComponent{
                .position = glm::vec2{0.f, 420.f},
                .scale = glm::vec2{1.f}
            }
        );
        auto boxCollider = reg.emplace<BoxColliderComponent>(
            entity2,
            BoxColliderComponent{
                .width = 450,
                .height = 20,
            }
        );
        auto& physics2 = reg.emplace<PhysicsComponent>(
            entity2,
            PhysicsComponent{
                PhysicsAttributes{
                    .eType = RigidBodyType::DYNAMIC,
                    .density = 1000.f,
                    .friction = 0.5f,
                    .restituton = 0.0f,
                    .gravityScale = 0.f,
                    .position = transform2.position,
                    .scale = transform2.scale,
                    .boxSize = glm::vec2{boxCollider.width, boxCollider.height},
                    .bBoxShape = true,
                    .bFixedRotation = false
                }
            }
        );
        physics2.Init(pPhysicsWorld ,640, 480);*/

        /*renderer->DrawLine(ENGINE_RENDERING::Line{
            .p1 = glm::vec2{50.f}, 
            .p2 = glm::vec2{200.f}, 
            .color = ENGINE_RENDERING::Color{255, 0, 0, 255}
        });
        renderer->DrawLine(ENGINE_RENDERING::Line{
            .p1 = glm::vec2{200.f, 50.f}, 
            .p2 = glm::vec2{50.f, 200.f}, 
            .color = ENGINE_RENDERING::Color{0, 255, 0, 255}
        });
        renderer->DrawRect(ENGINE_RENDERING::Rect{
            .position = glm::vec2{300, 300},
            .width = 100,
            .height = 100,
            .color = ENGINE_RENDERING::Color{0, 0, 255, 255}
        });*/

        // comment in Renderer: m_Text.clear()
        //auto pFont = assetManager->GetFont("pixel");
        /*renderer->DrawText2D(
            ENGINE_RENDERING::Text{
                .position = glm::vec2{100.f, 50.f},
                .textStr = "This is some text",
                .pFont = pFont
            }
        );
        renderer->DrawText2D(
            ENGINE_RENDERING::Text{
                .position = glm::vec2{50.f, 250.f},
                .textStr = "testing the wrap functionality, this is some text, wrap is 550",
                .wrap = 550.f,
                .pFont = pFont
            }
        );*/

        return true;
    }


    bool Application::LoadShaders()
	{
        auto& assetManager = m_pRegistry->GetContext<std::shared_ptr<ENGINE_RESOURCES::AssetManager>>();
        
        if (!assetManager)
        {
            ENGINE_ERROR("Failed to get Asset Manager from the Registry Context");
            return false;
        }

        if (!assetManager->AddShader("basic", "assets/shaders/basicShader.vert", "assets/shaders/basicShader.frag"))
        {
            ENGINE_ERROR("Failed to add Basic-Shaders to Asset Manager");
            return false;
        }
        if (!assetManager->AddShader("color", "assets/shaders/colorShader.vert", "assets/shaders/colorShader.frag"))
        {
            ENGINE_ERROR("Failed to add Color-Shaders to Asset Manager");
            return false;
        }
        if (!assetManager->AddShader("circle", "assets/shaders/circleShader.vert", "assets/shaders/circleShader.frag"))
		{
			ENGINE_ERROR("Failed to add the Circle-Shaders to the asset manager");
			return false;
		}
        if (!assetManager->AddShader("font", "assets/shaders/fontShader.vert", "assets/shaders/fontShader.frag"))
		{
			ENGINE_ERROR("Failed to add the Font-Shaders to the asset manager");
			return false;
		}

        return true;
    }


    void Application::ProcessEvents()
    {
        auto& inputManager = ENGINE_CORE::InputManager::GetInstance();
        auto& keyboard = inputManager.GetKeyboard();
        auto& mouse = inputManager.GetMouse();

        while(SDL_PollEvent(&m_Event))
        {
            ImGui_ImplSDL2_ProcessEvent(&m_Event);
            
            switch(m_Event.type)
            {
                case SDL_QUIT:
                    m_bIsRunning = false;
                    break;
                case SDL_KEYDOWN:
                    if(m_Event.key.keysym.sym == SDLK_ESCAPE)
                        m_bIsRunning = false;
                    keyboard.OnKeyPressed(m_Event.key.keysym.sym);
                    break;
                case SDL_KEYUP:
                    keyboard.OnKeyReleased(m_Event.key.keysym.sym);
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    mouse.OnBtnPressed(m_Event.button.button);
                    break;
                case SDL_MOUSEBUTTONUP:
                    mouse.OnBtnReleased(m_Event.button.button);
                    break;
                case SDL_MOUSEWHEEL:
                    mouse.SetMouseWheelX(m_Event.wheel.x);
                    mouse.SetMouseWheelX(m_Event.wheel.y);
                    break;
                case SDL_MOUSEMOTION:
                    mouse.SetMouseMoving(true);
                case SDL_WINDOWEVENT:
                    switch (m_Event.window.event)
                    {
                    case SDL_WINDOWEVENT_SIZE_CHANGED:
                        m_pWindow->SetWidth(m_Event.window.data1);
                        m_pWindow->SetHeight(m_Event.window.data2);
                        break;
                    default:
                        break;
                    }
                default:
                    break;
            }
        }
    }

    void Application::Update()
    {
        auto& camera = m_pRegistry->GetContext<std::shared_ptr<ENGINE_RENDERING::Camera2D>>();
        
        if (!camera)
        {
            ENGINE_ERROR("Failed to get Camera from the Registry Context");
            return;
        }
        
        camera->Update();
        auto& scriptSystem = m_pRegistry->GetContext<std::shared_ptr<ENGINE_CORE::Systems::ScriptingSystem>>();
        scriptSystem->Update();

        auto& physicsWorld = m_pRegistry->GetContext<ENGINE_PHYSICS::PhysicsWorld>();
        physicsWorld->Step(
            1.f / 60.f,
            10, 
            8
        );

        auto& physicsSystem = m_pRegistry->GetContext<std::shared_ptr<ENGINE_CORE::Systems::PhysicsSystem>>();
        physicsSystem->Update(m_pRegistry->GetRegistry());

        auto& animationSystem = m_pRegistry->GetContext<std::shared_ptr<ENGINE_CORE::Systems::AnimationSystem>>();
        animationSystem->Update();

        // Update inputs
        auto& inputManager = ENGINE_CORE::InputManager::GetInstance();
        auto& keyboard = inputManager.GetKeyboard();
        keyboard.Update();
        auto& mouse = inputManager.GetMouse();
        mouse.Update();

    }


    void Application::Render()
	{
        auto& renderSystem = m_pRegistry->GetContext<std::shared_ptr<ENGINE_CORE::Systems::RenderSystem>>();
        auto& renderUISystem = m_pRegistry->GetContext<std::shared_ptr<ENGINE_CORE::Systems::RenderUISystem>>();
        auto& renderShapeSystem = m_pRegistry->GetContext<std::shared_ptr<ENGINE_CORE::Systems::RenderShapeSystem>>();

        auto& camera = m_pRegistry->GetContext<std::shared_ptr<ENGINE_RENDERING::Camera2D>>();
        auto& renderer = m_pRegistry->GetContext<std::shared_ptr<ENGINE_RENDERING::Renderer>>();
        auto& assetManager = m_pRegistry->GetContext<std::shared_ptr<ENGINE_RESOURCES::AssetManager>>();
        
        auto shader = assetManager->GetShader("color");
        auto circleShader = assetManager->GetShader("circle");
        auto fontShader = assetManager->GetShader("font");

        glViewport(0, 0, m_pWindow->GetWidth(), m_pWindow->GetHeight());
        glClearColor(0.15f, 0.45f, 0.75f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        //renderer->SetViewport(0, 0, m_pWindow->GetWidth(), m_pWindow->GetHeight());
		//renderer->SetClearColor(0.15f, 0.45f, 0.75f, 1.f);
		//renderer->ClearBuffers(true, false, false);

        auto& scriptSystem = m_pRegistry->GetContext<std::shared_ptr<ENGINE_CORE::Systems::ScriptingSystem>>();
        scriptSystem->Render();
        renderSystem->Update();
        renderShapeSystem->Update();
        renderUISystem->Update(m_pRegistry->GetRegistry());

        Begin();
        RenderImGui();
        End();

        renderer->DrawLines(*shader, *camera);
        renderer->DrawFilledRects(*shader, *camera);
        renderer->DrawCircles(*circleShader, *camera);
        renderer->DrawAllText(*fontShader, *camera);

        SDL_GL_SwapWindow(m_pWindow->GetWindow().get());

        renderer->ClearPrimitives();
	}


    void Application::CleanUp()
	{
        SDL_Quit();
	}

    void Application::Run()
    {
        if (!Initialize())
        {
            ENGINE_ERROR("Initialization Failed");
            return;
        }

        while (m_bIsRunning)
        {
            ProcessEvents();
            Update();
            Render();
        }

        CleanUp();
    }


    bool Application::InitImGui()
    {
        const char* glslVersion = "#version 460";
        IMGUI_CHECKVERSION();

        if(!ImGui::CreateContext())
        {
            ENGINE_ERROR("Failed to create ImGui Context");
            return false;
        }

        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

        io.ConfigWindowsMoveFromTitleBarOnly = true;

        if(!ImGui_ImplSDL2_InitForOpenGL( m_pWindow->GetWindow().get(), m_pWindow->GetGLContext() ))
        {
            ENGINE_ERROR("Failed to initialize ImGui-SDL2-for-OpenGL");
            return false;
        }

        if(!ImGui_ImplOpenGL3_Init(glslVersion))
        {
            ENGINE_ERROR("Failed to initialize ImGui-OpenGL3");
            return false;   
        }

        return true;
    }

	void Application::Begin()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();
    }

	void Application::End()
    {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        ImGuiIO& io = ImGui::GetIO();
        if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            SDL_GLContext backupContext = SDL_GL_GetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			
			SDL_GL_MakeCurrent(
				m_pWindow->GetWindow().get(),
				backupContext
			);
        }
    }

	void Application::RenderImGui()
    {
        ImGui::ShowDemoWindow();
    }


    void Application::GetOpenGLVersionInfo()
    {
        std::cout <<"\nVendor: " <<glGetString(GL_VENDOR) <<std::endl;
        std::cout <<"Renderer: " <<glGetString(GL_RENDERER) <<std::endl;
        std::cout <<"Version: " <<glGetString(GL_VERSION) <<std::endl;
        std::cout <<"Shadong Language: " <<glGetString(GL_SHADING_LANGUAGE_VERSION) <<std::endl;
        std::cout <<"\n";
    }

}
     