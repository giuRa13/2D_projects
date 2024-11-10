#pragma once

#include "Core/ECS/Registry.hpp"
#include <Rendering/Core/BatchRenderer.hpp>


namespace ENGINE_CORE::Systems
{

    class RenderSystem
    {
    
    private:
        std::unique_ptr<ENGINE_RENDERING::SpriteBatchRenderer> m_pBatchRenderer;
    

    public:
        RenderSystem();
        ~RenderSystem() = default;

        void Update(ENGINE_CORE::ECS::Registry& m_Registry);
    };

}