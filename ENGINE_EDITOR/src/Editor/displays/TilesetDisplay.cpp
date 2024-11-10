#include "TilesetDisplay.hpp"
#include <Logger/Logger.hpp>
#include "Core/Resources/AssetManager.hpp"
#include "Core/ECS/MainRegistry.hpp"
#include <EngineUtils/EngineUtilities.hpp>
#include <imgui.h>


namespace ENGINE_EDITOR
{

    void TilesetDisplay::Draw()
    {
        if(!ImGui::Begin("Tileset"))
        {
            ImGui::End();
            return;
        }

        auto& assetManager = MAIN_REGISTRY().GetAssetManager();

        if(ImGui::BeginCombo("Chose Tileset", m_sTileset.c_str()))
        {
            for (const auto& sTileset : assetManager.GetTilesetNames())
            {
                bool bIsSelected = m_sTileset == sTileset;
                if(ImGui::Selectable(sTileset.c_str(), bIsSelected))
                    m_sTileset = sTileset;

                if(bIsSelected)
                    ImGui::SetItemDefaultFocus(); 
            }
            ImGui::EndCombo();
        }
        if(m_sTileset.empty())
        {
            ImGui::End();
            return;
        }

        auto pTexture = assetManager.GetTexture(m_sTileset);
        if(!pTexture)
        {
            ENGINE_ERROR("Loaded Tileset Texture is invvalide");
            ImGui::End();
            return;
        }
        int textureWidth = pTexture->GetWidth();
        int textureHeight = pTexture->GetHeight();
        int cols = textureWidth / 16;
        int rows = textureHeight / 16;
        float uv_w = 16 / static_cast<float>(textureWidth);
        float uv_h = 16 / static_cast<float>(textureHeight);
        float ux{0.f}, uy{0.f}, vx{uv_w}, vy{uv_h}; // UVs value are between 0 - 1

        ImGuiTableFlags tableFlags{0};
        tableFlags |= ImGuiTableFlags_SizingFixedFit;
        tableFlags |= ImGuiTableFlags_ScrollX;
        int k{0}, id{0};

        if(ImGui::BeginTable("Tileset", cols, tableFlags))
        {
            for (int i = 0; i < rows; i++)
            {
                ImGui::TableNextRow();
                for (int j = 0; j < cols; j++)
                {
                    ImGui::TableSetColumnIndex(j);

                    if(m_Selected == id)
                        ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, ImGui::GetColorU32(ImVec4{0.9f, 0.5f, 0.8f, 0.3f}));

                    // create unique ID for buttons
                    ImGui::PushID(k++);

                    if(ImGui::ImageButton((ImTextureID)pTexture->GetID(), 
                        ImVec2{16.f * 1.5, 16.f * 1.5}, //size
                        ImVec2{ux, uy},
                        ImVec2{vx, vy}))
                    {
                        m_Selected = id;
                        ENGINE_MESSAGE("StartX: {}, StartY: {}", j, i);
                    }
                    ImGui::PopID();

                    // Advance UVs to next Column
                    ux += uv_w;
                    vx += uv_w;
                    ++id;
                }
                // Set UVs to next Row
                ux = 0.f;
                vx = uv_w;
                uy += uv_h;
                vy += uv_h;
            }
            ImGui::EndTable();
        }

        ImGui::End();
    }

}