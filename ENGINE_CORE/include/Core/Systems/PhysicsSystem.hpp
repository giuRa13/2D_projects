#pragma once

#include "Core/ECS/Registry.hpp"


namespace ENGINE_CORE::Systems
{

    class PhysicsSystem
    {
    private:
        ENGINE_CORE::ECS::Registry& m_Registry;

    public:
        PhysicsSystem( ENGINE_CORE::ECS::Registry& registry );
        ~PhysicsSystem() = default;

        void Update( entt::registry& registry );
    };

} 