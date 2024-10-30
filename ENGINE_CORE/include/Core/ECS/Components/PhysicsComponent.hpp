#pragma once

#include <Physics/Box2DWrappers.hpp>
#include <sol/sol.hpp>
#include <glm/glm.hpp>
#include <entt/entt.hpp>


namespace ENGINE_CORE::ECS
{
    
    enum class RigidBodyType { STATIC = 0, KINEMATIC, DYNAMIC};

    struct PhysicsAttributes
    {
        RigidBodyType eType{RigidBodyType::STATIC};
        float density{1.f}, friction{0.2f}, restituton{0.2f};
        float restitutionThreshold{1.f}, radius{0.f}, gravityScale{1.f};

        glm::vec2 position{0.f}, scale{1.f}, boxSize{0.f}, offset{0.f};
        bool bCircle{false}, bBoxShape{true}, bFixedRotation{true}, bIsSensor{false};

        uint16_t filterCategory{0}, filterMask{0};
        int16_t groupIndex{0};
    };


    class PhysicsComponent
    {

        std::shared_ptr<b2Body> m_pRigidBody;
        PhysicsAttributes m_InitialAttribs;

    
    public:
        PhysicsComponent();
        PhysicsComponent(const PhysicsAttributes& physicsAttrib);
        ~PhysicsComponent() = default;

        void Init(ENGINE_PHYSICS::PhysicsWorld pPhysicsWorld, int windowWidth, int windowHeight);
        b2Body* GetBody() { return m_pRigidBody.get(); }

        const bool IsSensor() const;

        static void CreatePhysicsLuaBind(sol::state& lua, entt::registry& registry);

    };

}