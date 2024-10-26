#pragma once

#include "Core/ECS/Registry.hpp"
#include <Rendering/Core/BatchRenderer.hpp>


namespace ENGINE_CORE::Systems
{

    class RenderSystem
    {
    
    private:
        ENGINE_CORE::ECS::Registry& m_Registry;
        std::unique_ptr<ENGINE_RENDERING::SpriteBatchRenderer> m_pBatchRenderer;
    

    public:
        RenderSystem(ENGINE_CORE::ECS::Registry& registry);
        ~RenderSystem() = default;

        void Update();
    };

}