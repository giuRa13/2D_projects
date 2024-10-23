#pragma once

#include "Core/ECS/Registry.hpp"


namespace ENGINE_CORE::Systems
{

    class AnimationSystem
    {

    private:
        ENGINE_CORE::ECS::Registry& m_Registry;


    public:
        AnimationSystem(ENGINE_CORE::ECS::Registry& registry);
        ~AnimationSystem() = default;

        void Update();    
    
    };

}