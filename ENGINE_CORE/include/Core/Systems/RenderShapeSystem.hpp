#pragma once

#include "Core/ECS/Registry.hpp"
#include <Rendering/Core/RectBatchRenderer.hpp>
#include <Rendering/Core/CircleBatchRenderer.hpp>


namespace ENGINE_CORE::Systems
{

    class RenderShapeSystem
    {
    
    private:
        std::unique_ptr<ENGINE_RENDERING::RectBatchRenderer> m_pRectRenderer;
        std::unique_ptr<ENGINE_RENDERING::CircleBatchRenderer> m_pCircleRenderer;
    

    public:
        RenderShapeSystem();
        ~RenderShapeSystem() = default;

        void Update(ENGINE_CORE::ECS::Registry& registry);
    };

}