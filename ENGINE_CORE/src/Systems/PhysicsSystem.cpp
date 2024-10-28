#include "Core/Systems/PhysicsSystem.hpp"
#include "Core/ECS/Components/PhysicsComponent.hpp"
#include "Core/ECS/Components/BoxColliderComponent.hpp"
#include "Core/ECS/Components/CircleColliderComponent.hpp"
#include "Core/ECS/Components/TransformComponent.hpp"

using namespace ENGINE_CORE::ECS;


namespace ENGINE_CORE::Systems
{

    PhysicsSystem::PhysicsSystem(ENGINE_CORE::ECS::Registry& registry)
        : m_Registry{registry}
    {

    }

    void PhysicsSystem::Update(entt::registry& registry)
    {
        auto boxView = registry.view<PhysicsComponent, TransformComponent, BoxColliderComponent>();
        auto scaleWidth = 640.f / METERS_TO_PIXELS;
        auto scaleHeight = 480.f / METERS_TO_PIXELS;

        for (auto entity : boxView)
        {
            auto& physics = boxView.get<PhysicsComponent>(entity);
            auto pRigidBody = physics.GetBody();

            if(!pRigidBody)
                continue;

            auto& transform = boxView.get<TransformComponent>(entity);
            auto& boxCollider = boxView.get<BoxColliderComponent>(entity);

            const auto& bodyPosition = pRigidBody->GetPosition();

            transform.position.x = ((scaleWidth / 2.f) + bodyPosition.x) * METERS_TO_PIXELS - 
                (boxCollider.width * transform.scale.x) / 2.f - boxCollider.offset.x;

            transform.position.y = ((scaleHeight / 2.f) + bodyPosition.y) * METERS_TO_PIXELS - 
                (boxCollider.height * transform.scale.y) / 2.f - boxCollider.offset.y;

            if(!pRigidBody->IsFixedRotation())
                transform.rotation = glm::degrees(pRigidBody->GetAngle());
        }


        auto circleView = registry.view<PhysicsComponent, TransformComponent, CircleColliderComponent>();
        for (auto entity : circleView)
        {
            auto& physics = circleView.get<PhysicsComponent>(entity);
            auto pRigidBody = physics.GetBody();

            if(!pRigidBody)
                continue;

            auto& transform = boxView.get<TransformComponent>(entity);
            auto& circleCollider = circleView.get<CircleColliderComponent>(entity);

            const auto& bodyPosition = pRigidBody->GetPosition();

            transform.position.x = ((scaleWidth / 2.f) + bodyPosition.x) * METERS_TO_PIXELS - 
                (circleCollider.radius * transform.scale.x) - circleCollider.offset.x;

            transform.position.y = ((scaleHeight / 2.f) + bodyPosition.y) * METERS_TO_PIXELS - 
                (circleCollider.radius * transform.scale.y) - circleCollider.offset.y;

            if(!pRigidBody->IsFixedRotation())
                transform.rotation = glm::degrees(pRigidBody->GetAngle());
        }
    }

} 