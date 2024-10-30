#include "Core/Systems/RenderShapeSystem.hpp"
#include "Core/ECS/Components/BoxColliderComponent.hpp"
#include "Core/ECS/Components/CircleColliderComponent.hpp"
#include "Core/ECS/Components/TransformComponent.hpp"
#include "Core/ECS/Components/PhysicsComponent.hpp"
#include "Core/Resources/AssetManager.hpp"
#include <Rendering/Core/Camera2D.hpp>
#include <Rendering/Essentials/Primitives.hpp>
#include "Core/CoreUtilities/CoreEngineData.hpp"

using namespace ENGINE_CORE::ECS;
using namespace ENGINE_RENDERING;
using namespace ENGINE_RESOURCES;


namespace ENGINE_CORE::Systems
{

    RenderShapeSystem::RenderShapeSystem(ENGINE_CORE::ECS::Registry& registry)
        : m_Registry{registry},
        m_pRectRenderer{ std::make_unique<RectBatchRenderer>() },
        m_pCircleRenderer{ std::make_unique<CircleBatchRenderer>() }
    { }


    void RenderShapeSystem::Update()
    {
        if (!CoreEngineData::GetInstance().RenderCollidersEnabled())
			return;

        auto& camera = m_Registry.GetContext<std::shared_ptr<Camera2D>>();
        auto& assetManager = m_Registry.GetContext<std::shared_ptr<AssetManager>>();

        // Box
        auto colorShader = assetManager->GetShader("color");
        auto cam_mat = camera->GetCameraMatrix();

        colorShader->Enable();
        colorShader->SetUniformMat4("uProjection", cam_mat);
       
        m_pRectRenderer->Begin();

        auto boxView = m_Registry.GetRegistry().view<TransformComponent, BoxColliderComponent>();
        for(auto& entity : boxView)
        {
            const auto& transform = boxView.get<TransformComponent>(entity);
            const auto& boxCollider = boxView.get<BoxColliderComponent>(entity);
            glm::mat4 model{1.f};

            if  ( transform.rotation > 0.f || transform.rotation < 0.f || 
                  transform.scale.x > 1.f || transform.scale.x < 1.f ||
                  transform.scale.y > 1.f || transform.scale.y < 1.f)
            {
                model = glm::translate(model, glm::vec3{transform.position, 0.f});
                model = glm::translate(model, glm::vec3{(boxCollider.width * transform.scale.x) * 0.5f, (boxCollider.height * transform.scale.y) * 0.5f, 0.f});
                model = glm::rotate(model, glm::radians(transform.rotation), glm::vec3{0.f, 0.f, 1.f});
                model = glm::translate(model, glm::vec3{(boxCollider.width * transform.scale.x) * -0.5f, (boxCollider.height * transform.scale.y) * -0.5f, 0.f});
                    
                model = glm::scale(model, glm::vec3{transform.scale, 1.f});

                model = glm::translate(model, glm::vec3{-transform.position, 0.f});
             } 

            auto color = Color{255, 0, 0, 135};
            if (m_Registry.GetRegistry().all_of<PhysicsComponent>(entity))
            {
                auto& physics = m_Registry.GetRegistry().get<PhysicsComponent>(entity);
                if(physics.IsSensor())
                    color = Color{0, 255, 0, 135};
            }

            Rect rect{
                .position = glm::vec2{
                    transform.position.x + boxCollider.offset.x,
                    transform.position.y + boxCollider.offset.y,
                },
                .width = boxCollider.width * transform.scale.x,
                .height = boxCollider.height * transform.scale.y,
                .color = color
            };
            m_pRectRenderer->AddRect(rect, model);
        }
        m_pRectRenderer->End();
        m_pRectRenderer->Render();
        colorShader->Disable();


        // Circle
        auto circleShader = assetManager->GetShader("circle");

        circleShader->Enable();
        circleShader->SetUniformMat4("uProjection", cam_mat);
       
        m_pCircleRenderer->Begin();

        auto circleView = m_Registry.GetRegistry().view<TransformComponent, CircleColliderComponent>();
        for(auto& entity : circleView)
        {
            const auto& transform = circleView.get<TransformComponent>(entity);
            const auto& circleCollider = circleView.get<CircleColliderComponent>(entity);

            glm::vec4 circle{
                transform.position.x + circleCollider.offset.x,
                transform.position.y + circleCollider.offset.y,
                circleCollider.radius * transform.scale.x * 2,
                circleCollider.radius * transform.scale.y * 2
            };
            m_pCircleRenderer->AddCircle(circle, Color{0, 255, 0, 135}, 1.f);
        }
        m_pCircleRenderer->End();
        m_pCircleRenderer->Render();
        circleShader->Disable();
    }

}