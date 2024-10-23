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
#include <Rendering/Essentials/Vertex.hpp>
#include <Core/ECS/Entity.hpp>
#include <Core/ECS/Components/TransformComponent.hpp>
#include <Core/ECS/Components/SpriteComponent.hpp>
#include <Core/ECS/Components/Identification.hpp>
#include <Core/Resources/AssetManager.hpp>

#include <Core/Systems/ScriptingSystem.hpp>
#include <Core/Systems/RenderSystem.hpp>
#include <Core/Systems/AnimationSystem.hpp>

#include <Core/Scripting/InputManager.hpp>
#include <Windowing/Inputs/Keyboard.hpp>
#include <Windowing/Inputs/Mouse.hpp>



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

        m_pWindow = std::make_unique<ENGINE_WINDOWING::Window>(
            "Test window", 
            640, 480, 
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
            true, 
            SDL_WINDOW_OPENGL
        );
    
        if ( !m_pWindow->GetWindow() )
        {
            ENGINE_ERROR("Failed to create the window!"); 
            return false;
        }

        m_pWindow->SetGLContext(SDL_GL_CreateContext(m_pWindow->GetWindow().get()));
        if (!m_pWindow->GetGLContext())
        {
            std::string error = SDL_GetError();
            ENGINE_ERROR("Failed to create OpenGL Context!", error);
            return false;
        }

        SDL_GL_MakeCurrent(m_pWindow->GetWindow().get(), m_pWindow->GetGLContext());
        SDL_GL_SetSwapInterval( 1 );

        if ( gladLoadGLLoader( SDL_GL_GetProcAddress ) == 0 )
        {
            ENGINE_ERROR("Failed to Initialize Glad");
            return false;
        }

        // Enable Alpha Blending
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        GetOpenGLVersionInfo();
    
        /////////////////////////////////////////////////////////////////
        auto assetManager = std::make_shared<ENGINE_RESOURCES::AssetManager>();
        if(!assetManager)
        {
            ENGINE_ERROR("Failed to create the Asset manager");
            return false;
        }
        // Texture //////////////////////
        /*if(!assetManager->AddTexture("robot", "./assets/textures/16map.png", true))
        {
            ENGINE_ERROR("Failed to Create and Add Texture");
            return false;
        }*/
        /*if(!assetManager->AddTexture("robot", "./assets/textures/robotSprite.png", true))
        {
            ENGINE_ERROR("Failed to Create and Add Texture");
            return false;
        }*/
  
        m_pRegistry = std::make_unique<ENGINE_CORE::ECS::Registry>();

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

        // Shaders ( Application::LoadShaders())
        if(!LoadShaders())
        {
            ENGINE_ERROR("Failed to load the Shaders");
            return false;
        }

        ENGINE_CORE::Systems::ScriptingSystem::RegisterLuaBinding(*lua, *m_pRegistry);

        ENGINE_CORE::Systems::ScriptingSystem::RegisterLuaFunctions(*lua);

        if(!scriptSystem->LoadMainScript(*lua))
        {
            ENGINE_ERROR("Failed to load the Main Lua Script");
            return false; 
        }

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
            ENGINE_ERROR("Failed to add Shaders to Asset Manager");
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
            switch(m_Event.type)
            {
                case SDL_QUIT:
                    m_bIsRunning = false;
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

        auto& animationSystem = m_pRegistry->GetContext<std::shared_ptr<ENGINE_CORE::Systems::AnimationSystem>>();
        animationSystem->Update();

        // Update inputs
        auto& inputManager = ENGINE_CORE::InputManager::GetInstance();
        auto& keyboard = inputManager.GetKeyboard();
        keyboard.Update();
        auto& mouse = inputManager.GetMouse();
        mouse.Update();

        /*auto view = m_pRegistry->GetRegistry().view<ENGINE_CORE::ECS::TransformComponent, ENGINE_CORE::ECS::SpriteComponent>();
        static float rotation{0.f};
        static float x_pos{10.f};
        static bool bMoveRight{true};
        if(rotation >= 360.f)
            rotation = 0.f;

        if(bMoveRight && x_pos < 300)
            x_pos += 3; 
        else if (bMoveRight && x_pos >= 300.f)
            bMoveRight = false;

        if(!bMoveRight && x_pos > 10.f)
            x_pos -= 3;
        else if (!bMoveRight && x_pos <= 10.f)
            bMoveRight = true;

        for(const auto& entity : view)
        {
            ENGINE_CORE::ECS::Entity ent{*m_pRegistry, entity};
            auto& transform = ent.GetComponent<ENGINE_CORE::ECS::TransformComponent>();

            transform.rotation = rotation;
            transform.position.x = x_pos;
        }

        rotation += bMoveRight ? 9 : -9;*/
    }


    void Application::Render()
	{
        auto& renderSystem = m_pRegistry->GetContext<std::shared_ptr<ENGINE_CORE::Systems::RenderSystem>>();

        glViewport(0, 0, m_pWindow->GetWidth(), m_pWindow->GetHeight());
        glClearColor(0.15f, 0.45f, 0.75f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        auto& scriptSystem = m_pRegistry->GetContext<std::shared_ptr<ENGINE_CORE::Systems::ScriptingSystem>>();
        scriptSystem->Render();
        renderSystem->Update();

        SDL_GL_SwapWindow(m_pWindow->GetWindow().get());
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


    void Application::GetOpenGLVersionInfo()
    {
        std::cout <<"\nVendor: " <<glGetString(GL_VENDOR) <<std::endl;
        std::cout <<"Renderer: " <<glGetString(GL_RENDERER) <<std::endl;
        std::cout <<"Version: " <<glGetString(GL_VERSION) <<std::endl;
        std::cout <<"Shadong Language: " <<glGetString(GL_SHADING_LANGUAGE_VERSION) <<std::endl;
        std::cout <<"\n";
    }

}
     