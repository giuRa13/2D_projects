#include "SceneDisplay.hpp"
#include <imgui.h>
#include <Rendering/Buffers/FrameBuffer.hpp>


namespace ENGINE_EDITOR
{

    SceneDisplay::SceneDisplay(ENGINE_CORE::ECS::Registry& registry)
        : m_Registry{registry}
    {  }


    void SceneDisplay::Draw()
    {
        static bool pOpen{true};
        if(!ImGui::Begin("Scene", & pOpen))
        {
            ImGui::End();
            return;
        }

        if (ImGui::BeginChild("SceneChild", ImVec2{0.f, 0.f}, NULL, ImGuiWindowFlags_NoScrollWithMouse))
        {
            const auto& fb = m_Registry.GetContext<std::shared_ptr<ENGINE_RENDERING::FrameBuffer>>();

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