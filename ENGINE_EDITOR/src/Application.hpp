#pragma once

#include <glad/glad.h>
#include <Windowing/Window/Window.hpp>
#include <Core/ECS/Registry.hpp>
#include <memory>


namespace ENGINE_EDITOR
{

    class Application
    {
    
    private:
		std::unique_ptr<ENGINE_WINDOWING::Window> m_pWindow;
		std::unique_ptr<ENGINE_CORE::ECS::Registry> m_pRegistry;
		SDL_Event m_Event;
		bool m_bIsRunning;
	
    private:
		bool Initialize();
		bool LoadShaders();
		bool LoadEditorTextures();
		void ProcessEvents();

		void Update();
		void Render();
		void CleanUp();

		bool CreateDisplays();
		bool InitImGui();
		void Begin();
		void End();
		void RenderImGui();

		Application();
		~Application() = default;
		Application( const Application& ) = delete;
		Application& operator=( const Application& ) = delete;


    public:
		static Application& GetInstance();
		
        void Run();

        void GetOpenGLVersionInfo();
    
    };

}