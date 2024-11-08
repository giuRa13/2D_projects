#include "GridSystem.hpp"
#include <Core/ECS/MainRegistry.hpp>
#include <Core/Resources/AssetManager.hpp>
#include <Rendering/Core/RectBatchRenderer.hpp>
#include <Rendering/Core/Camera2D.hpp>
#include <Rendering/Essentials/Primitives.hpp>
#include <Rendering/Essentials/Vertex.hpp>


namespace ENGINE_EDITOR
{

    GridSystem::GridSystem()
        : m_pBatchRenderer{ std::make_unique<ENGINE_RENDERING::RectBatchRenderer>() }
    { }


    void GridSystem::Update(ENGINE_RENDERING::Camera2D& camera)
    {
        auto& mainRegistry = MAIN_REGISTRY();
        auto& assetManager = mainRegistry.GetAssetManager();

        auto camMat = camera.GetCameraMatrix();
        auto pColorShader = assetManager.GetShader("color");
        pColorShader->Enable();
        pColorShader->SetUniformMat4("uProjection", camMat);
        m_pBatchRenderer->Begin();

        int tileWidth{32}, tileHeight{32}, canvasWidth{640}, canvasHeight{480};
        int cols = canvasWidth / tileWidth;
        int rows = canvasHeight / tileHeight;

        ENGINE_RENDERING::Color color{};

        for (int row = 0; row < cols; row++)
        {
            for (int col = 0; col < cols; col++)
            {
                if( (col -row) % 2 == 0 && (row -col) % 2 == 0)
                    color = {125, 125, 125, 70};
                else
                    color = { 200, 200, 200, 70};

                ENGINE_RENDERING::Rect rect{
                    .position = glm::vec2{
                        static_cast<float>(col * tileWidth), 
                        static_cast<float>(row * tileHeight)
                    },
                    .width = static_cast<float>(tileWidth),
                    .height = static_cast<float>(tileHeight),
                    .color = color
                };

                m_pBatchRenderer->AddRect(rect);
            }   
        }

        m_pBatchRenderer->End();
        m_pBatchRenderer->Render();

        pColorShader->Disable();
    }
}