#define SDL_MAIN_HANDLED 1;

#include <Windowing/Window/Window.hpp>
#include <SDL.h>
#include <glad/glad.h>
#include <SOIL/SOIL.h>
#include <iostream>


void GetOpenGLVersionInfo()
{
    std::cout << "\nVendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "Shadong Language: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    std::cout << "\n";
}


bool LoadTexture(const std::string& filepath, int& width, int& height, bool blended)
{
    int channels = 0;

    unsigned char* image = SOIL_load_image(
        filepath.c_str(),
        &width, &height,
        &channels,
        SOIL_LOAD_AUTO
    );

    if (!image)
    {
        std::cout << "failed to load image [" << filepath << "]" <<SOIL_last_result();
        return false;
    }

    GLint format = GL_RGBA;

    switch (channels)
    {
    case 3: format = GL_RGB; break;
    case 4: format = GL_RGBA; break;
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    if (!blended)
    {
        //more pixelated
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    }
    else
    {
        //more smooth (blend pixel toghether)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }

    glTexImage2D(
        GL_TEXTURE_2D,
        0,               // level of detail (0=base image level)
        format,          // internal format ( number of color components)
        width, height,
        0,               // border
        format,          // format of the pixel data
        GL_UNSIGNED_BYTE,
        image
    );

    //stbi_image_free(image);
    SOIL_free_image_data(image);

    return true;
}


int main(int argc, char* argv[])
{
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

	ENGINE_WINDOWING::Window window("Test Window", 480, 480, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, true, SDL_WINDOW_OPENGL);
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


    GLuint texID;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);

    int width{ 0 }, height{ 0 };

    if (!LoadTexture("assets/textures/Dungeon_Character_at.png", width, height, false))
    {
        std::cout << "Failed to load Texture\n";
        return -1;
    }


    /////////////////////////////////////////////////////////////////////////////////////////////
    /*float vertices[] =
    {
        -0.5f,  0.5f,  0.0f,   0.0f, 1.0f, //top left
         0.5f,  0.5f,  0.0f,   1.0f, 1.0f,  //top right
         0.5f, -0.5f,  0.0f,   1.0f, 0.0f, //bottom right
        -0.5f, -0.5f,  0.0f,   0.0f, 0.0f, //bottom left
    };*/ //swapped tex coords
    float vertices[] =
    {
        -0.5f,  0.5f,  0.0f,   0.0f, 0.0f, //top left
         0.5f,  0.5f,  0.0f,   1.0f, 0.0f,  //top right
         0.5f, -0.5f,  0.0f,   1.0f, 1.0f, //bottom right
        -0.5f, -0.5f,  0.0f,   0.0f, 1.0f, //bottom left
    };
    GLuint indices[] =
    {
        0, 1, 2,
        2, 3, 0
    };
    const char* vertexSource =
        "#version 460 core\n"
        "layout (location = 0) in vec3 aPosition;\n"
        "layout (location = 1) in vec2 aTexCoords;\n"
        "out vec2 fragUVs;\n"
        "void main()\n"
        "{\n"
        "    gl_Position = vec4(aPosition, 1.0);\n"
        "    fragUVs = aTexCoords;\n"
        "}\0";
    GLuint vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER); //Create
    glShaderSource(vertexShader, 1, &vertexSource, NULL); //Add Source
    glCompileShader(vertexShader); //Compile

    int status;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
    if (!status)
    {
        char infoLog[1024];
        glGetShaderInfoLog(vertexShader, 1024, NULL, infoLog);
        std::cout << "Failed to Compile Vertex Shader \n" << infoLog << std::endl;
        return -1;
    }

    //
    const char* fragmentSource =
        "#version 460 core\n"
        "in vec2 fragUVs;"
        "out vec4 color;\n"
        "uniform sampler2D uTexture;\n"
        "void main()\n"
        "{\n"
        //"   color = vec4(1.0f, 0.5f, 0.0f, 1.0f);\n"
        "   color = texture(uTexture, fragUVs);\n"
        "}\0";
    GLuint fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); //Create
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL); //Add Source
    glCompileShader(fragmentShader); //Compile

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
    if (!status)
    {
        char infoLog[1024];
        glGetShaderInfoLog(fragmentShader, 1024, NULL, infoLog);
        std::cout << "Failed to Compile Fragment Shader \n" << infoLog << std::endl;
        return -1;
    }

    // Create, Link Program
    GLuint shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &status);
    if (!status)
    {
        char infoLog[1024];
        glGetProgramInfoLog(shaderProgram, 1024, NULL, infoLog);
        std::cout << "Failed to Link Shader Program \n" << infoLog << std::endl;
        return -1;
    }
    glUseProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // VAO VBO EBO
    GLuint VAO, VBO, IBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) * 3 * sizeof(float), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLuint), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(
        0, //layout 0 
        3, //3 vertices
        GL_FLOAT,
        GL_FALSE, //normalized false
        5 * sizeof(float),
        (void*)0); //offset of the first component 

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(
        1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
        reinterpret_cast<void*>(sizeof(float) * 3)
    );

    glEnableVertexAttribArray(1);

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

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texID);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        glBindVertexArray(0);

		SDL_GL_SwapWindow(window.GetWindow().get());
	}

	return 0;
}

