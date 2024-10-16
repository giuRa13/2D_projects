#define SDL_MAIN_HANDLED 1;

#include <Windowing/Window/Window.hpp>
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



struct UVs
{
    float u, v, width, height;
    UVs(): u{ 0.0f }, v{ 0.0f }, width{ 0.0f }, height{ 0.0f }
    { }
};

void GetOpenGLVersionInfo()
{
    std::cout << "\nVendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "Shadong Language: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    std::cout << "\n";
}



int main(int argc, char* argv[]) //int argc, char* argv[]
{
    ENGINE_INIT_LOGS(true, true);

	bool running{ true };

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		std::string error = SDL_GetError();
		std::cout << "Failed to init SDL: " << error << "\n";
		running = false;
	}

	if (SDL_GL_LoadLibrary(NULL) != 0)
	{
		std::string error = SDL_GetError();
		std::cout << "Failed to Open GL library: " << error << "\n";
		running = false;
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

	ENGINE_WINDOWING::Window window("Test Window", 640, 480, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, true, SDL_WINDOW_OPENGL);
	if (!window.GetWindow())
	{
		std::string error = SDL_GetError();
		std::cout << "Failed to Open GL library: " << error << "\n";
		return -1;
	}

	window.SetGLContext(SDL_GL_CreateContext(window.GetWindow().get()));
	if (!window.GetGLContext())
	{
		std::string error = SDL_GetError();
		std::cout << "Failed to create OpenGL context: " << error << "\n";
		running = false;
		return -1;
	}
	SDL_GL_MakeCurrent(window.GetWindow().get(), window.GetGLContext());
	SDL_GL_SetSwapInterval(1);

	if (gladLoadGLLoader(SDL_GL_GetProcAddress) == 0)
	{
		std::string error = SDL_GetError();
		std::cout << "Failed to load Glad: " << error << "\n";
		running = false;
		return -1;
	}
    
    // Enable Alpha Blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    /////////////////////////////////////////////////////////////////////////////////////////////
    
    auto texture = ENGINE_RENDERING::TextureLoader::Create(ENGINE_RENDERING::Texture::TextureType::PIXEL, "./assets/textures/16map.png");
    if(!texture)
    { 
        ENGINE_ERROR("Failed to create Texture");
        return -1;
    }

    UVs uVs{};
    ENGINE_LOG("Loaded Texture: [width = {0}, height = {1}]", texture->GetWidth(), texture->GetHeight());

    auto generateUVs = [&](float startX, float startY, float spriteWidth, float spriteHeight)
    {
            uVs.width = spriteWidth / texture->GetWidth();
            uVs.height = spriteHeight / texture->GetHeight();
            
            uVs.u = startX * uVs.width;
            uVs.v = startY * uVs.height;
    };

    generateUVs(13, 10, 16, 16);

    /////////////////////////////////////////////////////////////////////////////////////////////
    /*float vertices[] =
    {
        -0.5f,  0.5f,  0.0f,   0.0f, 1.0f, //top left            //swapped tex coords 0.0f, 0.0f,
         0.5f,  0.5f,  0.0f,   1.0f, 1.0f,  //top right                               1.0f, 0.0f,
         0.5f, -0.5f,  0.0f,   1.0f, 0.0f, //bottom right                             1.0f, 1.0f,
        -0.5f, -0.5f,  0.0f,   0.0f, 0.0f, //bottom left                              0.0f, 1.0f
    };*/ 
    /*float vertices[] =                                // Before Camera projection
    {
        -0.5f,  0.5f,  0.0f,   uvs.u,   (uvs.v + uvs.height),              // TL (0, 1)
         0.5f,  0.5f,  0.0f,   uvs.u,   uvs.v,                             // BL (0, 0)
         0.5f, -0.5f,  0.0f,   (uvs.u + uvs.width),   uvs.v,                // BR (1, 0)
        - 0.5f, -0.5f,  0.0f,  (uvs.u + uvs.width), (uvs.v + uvs.height)   // TR (0, 0)
    };*/
    /*float vertices[] =
    {
         10.f,  26.f,  0.0f,   uvs.u,   (uvs.v + uvs.height),              // TL (0, 1)
         10.f,  10.f,  0.0f,   uvs.u,   uvs.v,                             // BL (0, 0)
         26.f,  10.f,  0.0f,   (uvs.u + uvs.width),   uvs.v,                // BR (1, 0)
         26.f,  26.f,  0.0f,  (uvs.u + uvs.width), (uvs.v + uvs.height)   // TR (0, 0)
    };*/
    std::vector<ENGINE_RENDERING::Vertex> vertices{};
    ENGINE_RENDERING::Vertex vTL{}, vTR{}, vBL{}, vBR{};

    vTL.position = glm::vec2{10.f, 26.f};
    vTL.uvs = glm::vec2{ uVs.u,   (uVs.v + uVs.height) };

    vTR.position = glm::vec2{ 10.f, 10.f };
    vTR.uvs = glm::vec2{ uVs.u,   uVs.v };
    
    vBL.position = glm::vec2{ 26.f, 10.f };
    vBL.uvs = glm::vec2{ (uVs.u + uVs.width),  uVs.v };
    
    vBR.position = glm::vec2{ 26.f, 26.f };
    vBR.uvs = glm::vec2{ (uVs.u + uVs.width), (uVs.v + uVs.height) };

    vertices.push_back(vTL);
    vertices.push_back(vTR);
    vertices.push_back(vBL);
    vertices.push_back(vBR);

    GLuint indices[] =
    {
        0, 1, 2,
        2, 3, 0
    };

    ENGINE_RENDERING::Camera2D camera{};
    camera.SetScale(5.f);

    auto shader = ENGINE_RENDERING::ShaderLoader::Create(
        "assets/shaders/basicShader.vert", 
        "assets/shaders/basicShader.frag");
    if (!shader)
    {
        ENGINE_ERROR("Failete to Create shaders");
        return -1;
    }


    // VAO VBO EBO
    GLuint VAO, VBO, IBO;
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
    /////////////////////////////////////////////////////////////////////////////////////////////////


    GetOpenGLVersionInfo();


	SDL_Event event{};
	while (running)
	{
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				running = false;
				break;
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE)
					running = false;
				break;
			default:
				break;
			}
		}

		glViewport(0, 0, window.GetWidth(), window.GetHeight());
		glClearColor(0.15f, 0.45f, 0.75f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

        shader->Enable();
        glBindVertexArray(VAO);

        auto projection = camera.GetCameraMatrix();
        shader->SetUniformMat4("uProjection", projection);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture->GetID());

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        glBindVertexArray(0);

		SDL_GL_SwapWindow(window.GetWindow().get());

        camera.Update();
        shader->Disable();
	}

	return 0;
}

