#include "SceneDisplay.hpp"
#include <imgui.h>
#include <Rendering/Buffers/FrameBuffer.hpp>
#include <Rendering/Core/Camera2D.hpp>
#include <Rendering/Core/Renderer.hpp>
#include <Core/ECS/MainRegistry.hpp>
#include <Core/Systems/AnimationSystem.hpp>
#include <Core/Systems/PhysicsSystem.hpp>
#include <Core/Systems/ScriptingSystem.hpp>
#include <Core/CoreUtilities/CoreEngineData.hpp>
#include <Logger/Logger.hpp>
#include <Core/Systems/RenderSystem.hpp>
#include <Core/Systems/RenderUISystem.hpp>
#include <Core/Systems/RenderShapeSystem.hpp>
#include <Sounds/MusicPlayer/MusicPlayer.hpp>
#include <Sounds/SoundPlayer/SoundFxPlayer.hpp>
#include <Physics/Box2DWrappers.hpp>
#include <Core/Resources/AssetManager.hpp>
#include "Editor/utilities/EditorFramebuffer.hpp"

using namespace ENGINE_CORE::Systems;

constexpr float one_over_sixty = 1.f / 60.f;


namespace ENGINE_EDITOR
{

    SceneDisplay::SceneDisplay(ENGINE_CORE::ECS::Registry& registry)
        : m_Registry{registry}, m_bPlayScene{false}, m_bSceneLoaded{false}
    {  }


    void SceneDisplay::LoadScene()
    {
        auto& scriptSystem = m_Registry.GetContext<std::shared_ptr<ScriptingSystem>>();
        auto& lua = m_Registry.GetContext<std::shared_ptr<sol::state>>();

        if(!lua)
            lua = std::make_shared<sol::state>();

        lua->open_libraries(sol::lib::base, sol::lib::math, sol::lib::os, 
            sol::lib::table, sol::lib::io, sol::lib::string, sol::lib::package);

        ENGINE_CORE::Systems::ScriptingSystem::RegisterLuaBinding(*lua, m_Registry);
        ENGINE_CORE::Systems::ScriptingSystem::RegisterLuaFunctions(*lua, m_Registry);

        if(!scriptSystem->LoadMainScript(*lua))
        {
            ENGINE_ERROR("Failed to load the Main Lua Script");
            return; 
        }

        m_bSceneLoaded = true;
        m_bPlayScene = true;
    }

    void SceneDisplay::UnloadScene()
    {
        m_bPlayScene = false;
        m_bSceneLoaded = false;
        m_Registry.GetRegistry().clear();
        auto& lua = m_Registry.GetContext<std::shared_ptr<sol::state>>();
        lua.reset();

        auto& mainRegistry = MAIN_REGISTRY();
        mainRegistry.GetMusicPlayer().Stop();
        mainRegistry.GetSoundPlayer().Stop(-1);
    }


    void SceneDisplay::RenderScene()
    {
        auto& mainRegistry = MAIN_REGISTRY();
        auto& editorFrameBuffers = mainRegistry.GetContext<std::shared_ptr<EditorFramebuffers>>();
        auto& renderer = mainRegistry.GetContext<std::shared_ptr<ENGINE_RENDERING::Renderer>>();

        auto& renderSystem = mainRegistry.GetContext<std::shared_ptr<RenderSystem>>();
        auto& renderUISystem = mainRegistry.GetContext<std::shared_ptr<RenderUISystem>>();
        auto& renderShapeSystem = mainRegistry.GetContext<std::shared_ptr<RenderShapeSystem>>();

        const auto& fb = editorFrameBuffers->mapFramebuffers[FramebufferType::SCENE];
        
        fb->Bind();
        //glViewport(0, 0, m_pWindow->GetWidth(), m_pWindow->GetHeight());
        //glClearColor(0.15f, 0.45f, 0.75f, 1.0f);
        //glClear(GL_COLOR_BUFFER_BIT);
        //renderer->SetViewport(0, 0, m_pWindow->GetWidth(), m_pWindow->GetHeight());
        renderer->SetViewport(0, 0, fb->Width(), fb->Height());
        renderer->SetClearColor( 0.f, 0.f, 0.f, 1.f );//(0.15f, 0.45f, 0.75f, 1.f)
		renderer->ClearBuffers(true, true, false);

        renderSystem->Update();
        renderShapeSystem->Update();
        renderUISystem->Update(m_Registry.GetRegistry());
        
        fb->Unbind();
        fb->CheckResize();
    }


    void SceneDisplay::Update()
    {
        if(!m_bPlayScene)
            return;

        auto& mainRegistry = MAIN_REGISTRY();
        auto& coreGlobals = CORE_GLOBALS();

        // Camera
        auto& camera = m_Registry.GetContext<std::shared_ptr<ENGINE_RENDERING::Camera2D>>();
        if (!camera)
        {
            ENGINE_ERROR("Failed to get Camera from the Registry Context");
            return;
        }
        camera->Update();
        //Scripting
        auto& scriptSystem = m_Registry.GetContext<std::shared_ptr<ENGINE_CORE::Systems::ScriptingSystem>>();
        scriptSystem->Update();
        // Physics
        if(coreGlobals.IsPhysicsEnabled())
        {
            auto& pPhysicsWorld = m_Registry.GetContext<ENGINE_PHYSICS::PhysicsWorld>();
            pPhysicsWorld->Step(one_over_sixty,  coreGlobals.GetVelocityIterations(),  coreGlobals.GetPositionIterations());
            pPhysicsWorld->ClearForces();
        }
        auto& pPhysicsSystem = m_Registry.GetContext<std::shared_ptr<ENGINE_CORE::Systems::PhysicsSystem>>();
        pPhysicsSystem->Update(m_Registry.GetRegistry());
        // Animation
        auto& animationSystem = m_Registry.GetContext<std::shared_ptr<ENGINE_CORE::Systems::AnimationSystem>>();
        animationSystem->Update();
    }


    void SceneDisplay::Draw()
    {
        static bool pOpen{true};
        if(!ImGui::Begin("Scene", & pOpen))
        {
            ImGui::End();
            return;
        }

        auto& mainRegistry = MAIN_REGISTRY();
        auto& assetManager = mainRegistry.GetAssetManager();
        auto pPlayTexture = assetManager.GetTexture("play_button");
        auto pStopTexture = assetManager.GetTexture("stop_button"); 
        constexpr static auto green = ImVec4(0.0f, 9.0f, 0.0f, 0.3f);

        /*if ( m_bPlayScene )
        {
            ImGui::PushStyleColor( ImGuiCol_Button, ImVec4{ 0.f, 0.9f, 0.f, 0.3f } );
            ImGui::PushStyleColor( ImGuiCol_ButtonHovered, ImVec4{ 0.f, 0.9f, 0.f, 0.3f } );
            ImGui::PushStyleColor( ImGuiCol_ButtonActive, ImVec4{ 0.f, 0.9f, 0.f, 0.3f } );
        }*/
        if(ImGui::ImageButton((ImTextureID)pPlayTexture->GetID(),
            ImVec2{
                (float)pPlayTexture->GetWidth(),
                (float)pPlayTexture->GetHeight()
            }
        )   && !m_bSceneLoaded) 
        {
            LoadScene();
        }
        /*if(ImGui::GetColorStackSize() > 0)
            ImGui::PopStyleColor(ImGui::GetColorStackSize());
        if( ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
            ImGui::SetTooltip("Play Scene");*/

        ImGui::SameLine();

        /*if ( m_bPlayScene )
        {
            ImGui::PushStyleColor( ImGuiCol_Button, ImVec4{ 0.f, 0.9f, 0.f, 0.3f } );
            ImGui::PushStyleColor( ImGuiCol_ButtonHovered, ImVec4{ 0.f, 0.9f, 0.f, 0.3f } );
            ImGui::PushStyleColor( ImGuiCol_ButtonActive, ImVec4{ 0.f, 0.9f, 0.f, 0.3f } );
        }*/

       RenderScene();

        if(ImGui::ImageButton((ImTextureID)pStopTexture->GetID(),
            ImVec2{
                (float)pStopTexture->GetWidth(),
                (float)pStopTexture->GetHeight()
            }
        )   && m_bSceneLoaded) 
        {
            UnloadScene();
        }
        	//ImGui::PopStyleColor( 2 ); // ImGui::PushStyleColor called three times only. (m_bPlayScene) != (!m_bPlayScene)
        /*if(ImGui::GetColorStackSize() > 0)
            ImGui::PopStyleColor(ImGui::GetColorStackSize());
        if( ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
            ImGui::SetTooltip("Stop Scene");*/
        
        /////////////
        if (ImGui::BeginChild("##SceneChild", ImVec2{0.f, 0.f}, NULL, ImGuiWindowFlags_NoScrollWithMouse))
        {
            auto& editorFrameBuffers = mainRegistry.GetContext<std::shared_ptr<EditorFramebuffers>>();
            const auto& fb = editorFrameBuffers->mapFramebuffers[FramebufferType::SCENE];

            ImGui::SetCursorPos(ImVec2{0.f, 0.f});

            ImGui::Image( // id, size, uvs
                (ImTextureID)fb->GetTextureID(), 
                ImVec2{ static_cast<float>(fb->Width()), static_cast<float>(fb->Height()) },
                ImVec2{0.f, 1.f}, ImVec2{1.f, 0.f}
            );

            ImGui::EndChild();
            
            // check Resize based on window size
            ImVec2 windowSize{ ImGui::GetWindowSize()};
            if(fb->Width() != static_cast<int>(windowSize.x) || fb->Height() != static_cast<int>(windowSize.y))
                fb->Resize(static_cast<int>(windowSize.x), static_cast<int>(windowSize.y));
        }

        ImGui::End();
    }

}