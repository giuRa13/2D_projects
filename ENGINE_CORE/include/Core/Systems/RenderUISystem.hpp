#pragma once

#include <Rendering/Core/BatchRenderer.hpp>
#include <Rendering/Core/TextBatchRenderer.hpp>
#include <Rendering/Core/Camera2D.hpp>
#include "Core/ECS/Registry.hpp"


namespace ENGINE_CORE::Systems
{

    class RenderUISystem
    {

    private:
        std::unique_ptr<ENGINE_RENDERING::SpriteBatchRenderer> m_pSpriteRenderer;
        std::unique_ptr<ENGINE_RENDERING::TextBatchRenderer> m_pTextRenderer;
        std::unique_ptr<ENGINE_RENDERING::Camera2D> m_pCamera2D;


    public:
        RenderUISystem();
        ~RenderUISystem() = default;

        void Update(ENGINE_CORE::ECS::Registry& registry);

    };

}
