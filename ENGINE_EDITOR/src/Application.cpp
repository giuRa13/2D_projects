#include "Application.hpp"
#include <SDL.h>
#include <glad/glad.h>
#include <SOIL/SOIL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <Logger/Logger.hpp>
#include <Rendering/Essentials/ShaderLoader.hpp>
#include <Rendering/Essentials/TextureLoader.hpp>
#include <Rendering/Core/Camera2D.hpp>
#include <Rendering/Essentials/Vertex.hpp>
#include <Core/ECS/Entity.hpp>
#include <Core/ECS/Components/TransformConponent.hpp>
#include <Core/ECS/Components/SpriteComponent.hpp>
#include <Core/ECS/Components/Identification.hpp>
#include <Core/Resources/AssetManager.hpp>


namespace ENGINE_EDITOR
{

	bool Application::Initialize()
	{
        ENGINE_INIT_LOGS(true, true);

        if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        {
            std::string error = SDL_GetError();
            std::cout << "Failed to init SDL: " << error << "\n";
            return  false;
        }

        if (SDL_GL_LoadLibrary(NULL) != 0)
        {
            std::string error = SDL_GetError();
            std::cout << "Failed to Open GL library: " << error << "\n";
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
            "Test Window", 
            640, 480, 
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
            true, 
            SDL_WINDOW_OPENGL
        );

        if (!m_pWindow->GetWindow())
        {
            std::string error = SDL_GetError();
            std::cout << "Failed to Open GL library: " << error << "\n";
            return false;
        }

        m_pWindow->SetGLContext(SDL_GL_CreateContext(m_pWindow->GetWindow().get()));
        if (!m_pWindow->GetGLContext())
        {
            std::string error = SDL_GetError();
            std::cout << "Failed to create OpenGL context: " << error << "\n";
            return false;
        }
        SDL_GL_MakeCurrent(m_pWindow->GetWindow().get(), m_pWindow->GetGLContext());
        SDL_GL_SetSwapInterval(1);

        if (gladLoadGLLoader(SDL_GL_GetProcAddress) == 0)
        {
            std::string error = SDL_GetError();
            std::cout << "Failed to load Glad: " << error << "\n";
            return false;
        }

        // Enable Alpha Blending
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        /////////////////////////////////////////////////////////////////////////////////////////////

        auto assetManager = std::make_shared<ENGINE_RESOURCES::AssetManager>();
        if (!assetManager)
        {
            ENGINE_ERROR("Failed to create AssetManager");
            return -1;
        }

        if (!assetManager->AddTexture("16map", "./assets/textures/16map.png", true))
        {
            ENGINE_ERROR("Failed to create and add texture");
            return -1;
        }

        auto texture = assetManager->GetTexture("16map");
        ENGINE_LOG("Loaded Texture: [width = {0}, height = {1}]", texture.GetWidth(), texture.GetHeight());

        // Registry /////////////////////
        m_pRegistry = std::make_unique<ENGINE_CORE::ECS::Registry>();

        ENGINE_CORE::ECS::Entity entity1{ *m_pRegistry, "Ent1", "Test" };

        auto& transform = entity1.AddComponent<ENGINE_CORE::ECS::TransformComponent>(ENGINE_CORE::ECS::TransformComponent{
                .position = glm::vec2{10.f, 10.f},
                .scale = glm::vec2{1.f, 1.f},
                .rotation = 0.f
            }
        );

        auto& sprite = entity1.AddComponent<ENGINE_CORE::ECS::SpriteComponent>(ENGINE_CORE::ECS::SpriteComponent{
                .width = 16.f,
                .height = 16.f,
                .color = ENGINE_RENDERING::Color{.r = 255, .g = 0, .b = 255, .a = 255},
                .start_x = 13,
                .start_y = 13,
            }
            );

        sprite.generate_uvs(texture.GetWidth(), texture.GetHeight());

        /*float vertices[] =
        {
             10.f,  26.f,  0.0f,   uvs.u,   (uvs.v + uvs.height),              // TL (0, 1)
             10.f,  10.f,  0.0f,   uvs.u,   uvs.v,                             // BL (0, 0)
             26.f,  10.f,  0.0f,   (uvs.u + uvs.width),   uvs.v,                // BR (1, 0)
             26.f,  26.f,  0.0f,  (uvs.u + uvs.width), (uvs.v + uvs.height)   // TR (0, 0)
        };*/
        std::vector<ENGINE_RENDERING::Vertex> vertices{};
        ENGINE_RENDERING::Vertex vTL{}, vTR{}, vBL{}, vBR{};

        vTL.position = glm::vec2{ transform.position.x,    transform.position.y + sprite.height };
        vTL.uvs = glm::vec2{ sprite.uvs.u,   sprite.uvs.v + sprite.uvs.uv_height };

        vTR.position = glm::vec2{ transform.position.x + sprite.width,    transform.position.y + sprite.height };
        vTR.uvs = glm::vec2{ sprite.uvs.u + sprite.uvs.uv_width,   sprite.uvs.v + sprite.uvs.uv_height };

        vBL.position = glm::vec2{ transform.position.x,    transform.position.y };
        vBL.uvs = glm::vec2{ sprite.uvs.u,    sprite.uvs.v };

        vBR.position = glm::vec2{ transform.position.x + sprite.width,    transform.position.y };
        vBR.uvs = glm::vec2{ sprite.uvs.u + sprite.uvs.uv_width,    sprite.uvs.v };

        vertices.push_back(vTL);
        vertices.push_back(vBL);
        vertices.push_back(vBR);
        vertices.push_back(vTR);

        auto& id = entity1.GetComponent<ENGINE_CORE::ECS::Identification>();
        ENGINE_LOG("Name: {0}, Group: {1}, ID: {2}", id.name, id.group, id.entity_id);

        GLuint indices[] =
        {
            0, 1, 2,
            2, 3, 0
        };

        // Camera ////////////////////////////
        auto camera = std::make_shared<ENGINE_RENDERING::Camera2D>();
        camera->SetScale(5.f);

        if(!m_pRegistry->AddToContext<std::shared_ptr<ENGINE_RESOURCES::AssetManager>>(assetManager))
        {
            ENGINE_ERROR("Failed to add Asset Manager to the Registry Context");
            return false;
        }

        if (!m_pRegistry->AddToContext<std::shared_ptr<ENGINE_RENDERING::Camera2D>>(camera))
        {
            ENGINE_ERROR("Failed to add Camera to the Registry Context");
            return false;
        }

        if (!LoadShaders())
        {
            ENGINE_ERROR("Failed to load the Shaders");
            return false;
        }

        // VAO VBO EBO ///////////////////////
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(ENGINE_RENDERING::Vertex), vertices.data(), GL_STATIC_DRAW);

        glGenBuffers(1, &IBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLuint), indices, GL_STATIC_DRAW);

        // Vertices
        glVertexAttribPointer(
            0,                                                      //layout 0 
            2,                                                      //2 components
            GL_FLOAT,
            GL_FALSE,                                               //normalized false
            sizeof(ENGINE_RENDERING::Vertex),
            (void*)offsetof(ENGINE_RENDERING::Vertex, position)     //offset of the first component 
        );
        glEnableVertexAttribArray(0);

        // UVs
        glVertexAttribPointer(
            1,
            2,
            GL_FLOAT,
            GL_FALSE,
            sizeof(ENGINE_RENDERING::Vertex),
            (void*)offsetof(ENGINE_RENDERING::Vertex, uvs)
        );
        glEnableVertexAttribArray(1);

        // Color
        glVertexAttribPointer(
            2,
            4,
            GL_UNSIGNED_BYTE,
            GL_TRUE,                                            // Normalize to [0, 1] because of Unsigned Int 
            sizeof(ENGINE_RENDERING::Vertex),
            (void*)offsetof(ENGINE_RENDERING::Vertex, color)
        );
        glEnableVertexAttribArray(2);

        glBindVertexArray(0);
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
        while (SDL_PollEvent(&m_Event))
        {
            switch (m_Event.type)
            {
            case SDL_QUIT:
                m_bIsRunning = false;
                break;
            case SDL_KEYDOWN:
                if (m_Event.key.keysym.sym == SDLK_ESCAPE)
                    m_bIsRunning = false;
                break;
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
	}


	void Application::Render()
	{
        auto& assetManager = m_pRegistry->GetContext<std::shared_ptr<ENGINE_RESOURCES::AssetManager>>();
        auto& camera = m_pRegistry->GetContext<std::shared_ptr<ENGINE_RENDERING::Camera2D>>();

        auto& shader = assetManager->GetShader("basic");
        auto projection = camera->GetCameraMatrix();

        if (shader.ShaderProgramID() == 0)
        {
            ENGINE_ERROR("Shader Program has not be created correctly");
            return;
        }

        glViewport(0, 0, m_pWindow->GetWidth(), m_pWindow->GetHeight());
        glClearColor(0.15f, 0.45f, 0.75f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.Enable();
        glBindVertexArray(VAO);

        shader.SetUniformMat4("uProjection", projection); 

        glActiveTexture(GL_TEXTURE0);
        const auto& texture = assetManager->GetTexture("16map");
        glBindTexture(GL_TEXTURE_2D, texture.GetID());

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        glBindVertexArray(0);

        SDL_GL_SwapWindow(m_pWindow->GetWindow().get());

        shader.Disable();
	}


	void Application::CleanUp()
	{
        SDL_Quit();
	}

	Application::Application()
		: m_pWindow{nullptr}, m_pRegistry{nullptr}, m_Event{}, m_bIsRunning{true},
		VAO{0}, VBO{0}, IBO{0} //to remove later
	{ }

    Application& Application::GetInstance()
    {
        // "static" allows to do this only one time 
        static Application app{};
        return app;
    }

	Application::~Application()
	{ }
	
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

}