#pragma once

#include "Core/ECS/Registry.hpp"
#include <Rendering/Core/RectBatchRenderer.hpp>
#include <Rendering/Core/CircleBatchRenderer.hpp>


namespace ENGINE_CORE::Systems
{

    class RenderShapeSystem
    {
    
    private:
        ENGINE_CORE::ECS::Registry& m_Registry;
        std::unique_ptr<ENGINE_RENDERING::RectBatchRenderer> m_pRectRenderer;
        std::unique_ptr<ENGINE_RENDERING::CircleBatchRenderer> m_pCircleRenderer;
    

    public:
        RenderShapeSystem(ENGINE_CORE::ECS::Registry& registry);
        ~RenderShapeSystem() = default;

        void Update();
    };

}