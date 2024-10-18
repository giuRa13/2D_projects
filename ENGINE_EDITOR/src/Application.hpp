#pragma once

#include <glad/glad.h>
#include <Windowing/Window/Window.hpp>
#include <Core/ECS/Registry.hpp>


namespace ENGINE_EDITOR
{

	class Application
	{

	private:
		std::unique_ptr<ENGINE_WINDOWING::Window> m_pWindow;
		std::unique_ptr<ENGINE_CORE::ECS::Registry> m_pRegistry;

		SDL_Event m_Event;

		bool m_bIsRunning;

		// temporary
		GLuint VAO, VBO, IBO;

	private:
		bool Initialize();
		bool LoadShaders();

		void ProcessEvents();
		void Update();
		void Render();

		void CleanUp();

		Application();


	public:
		static Application& GetInstance();
		~Application();

		void Run();

	};

}