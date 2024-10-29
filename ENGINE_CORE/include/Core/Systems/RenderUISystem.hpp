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
        ENGINE_CORE::ECS::Registry& m_Registry;
        std::unique_ptr<ENGINE_RENDERING::SpriteBatchRenderer> m_pSpriteRenderer;
        std::unique_ptr<ENGINE_RENDERING::TextBatchRenderer> m_pTextRenderer;
        std::unique_ptr<ENGINE_RENDERING::Camera2D> m_pCamera2D;


    public:
        RenderUISystem(ENGINE_CORE::ECS::Registry& registry);
        ~RenderUISystem() = default;

        void Update(entt::registry& registry);

    };

}
