#include "Core/Systems/RenderSystem.hpp"
#include "Core/Resources/AssetManager.hpp"
#include "Core/ECS/Components/SpriteComponent.hpp"
#include "Core/ECS/Components/TransformComponent.hpp"
#include "Core/CoreUtilities/CoreUtilities.hpp"
#include <Rendering/Core/Camera2D.hpp>
#include <Rendering/Essentials/Shader.hpp>
#include "Core/ECS/MainRegistry.hpp"
#include <Logger/Logger.hpp>

using namespace ENGINE_CORE::ECS;
using namespace ENGINE_RENDERING;
using namespace ENGINE_RESOURCES;


namespace ENGINE_CORE::Systems
{

        RenderSystem::RenderSystem()
            : m_pBatchRenderer{ std::make_unique<SpriteBatchRenderer>() }
        { }

        void RenderSystem::Update(ENGINE_CORE::ECS::Registry& registry)
        {
            auto view = registry.GetRegistry().view<SpriteComponent, TransformComponent>();
		    if (view.size_hint() < 1)
			    return;

            auto& mainRegistry = MAIN_REGISTRY();
	        auto& assetManager = mainRegistry.GetAssetManager();
            //auto& assetManager = m_Registry.GetContext<std::shared_ptr<AssetManager>>();
            auto& camera = registry.GetContext<std::shared_ptr<Camera2D>>();
            

            const auto& spriteShader = assetManager.GetShader("basic");
            auto cam_mat = camera->GetCameraMatrix();

            if(spriteShader->ShaderProgramID() == 0)
            {
                ENGINE_ERROR("Sprite Shader Program has not been set correctly");
                return;
            } 

            spriteShader->Enable();
            spriteShader->SetUniformMat4("uProjection", cam_mat);

            m_pBatchRenderer->Begin();

            for (const auto& entity : view)
            {
                const auto& transform = view.get<TransformComponent>(entity);
                const auto& sprite = view.get<SpriteComponent>(entity);

                if (!ENGINE_CORE::EntityInView(transform, sprite.width, sprite.height, *camera))
				    continue;

                if(sprite.texture_name.empty() || sprite.bHidden)
                    continue;

                const auto& pTexture = assetManager.GetTexture(sprite.texture_name);
                if(!pTexture)
                {
                    ENGINE_ERROR("Texture [{0}] was not created correctly", sprite.texture_name);
                    return;
                }
                
                glm::vec4 spriteRect{transform.position.x, transform.position.y, sprite.width, sprite.height};
                glm::vec4 uvRect{sprite.uvs.u, sprite.uvs.v, sprite.uvs.uv_width, sprite.uvs.uv_height};

                glm::mat4 model = ENGINE_CORE::RSTModel(transform, sprite.width, sprite.height);

                /*glm::mat4 model{1.f};
                if  ( transform.rotation > 0.f || transform.rotation < 0.f || 
                    transform.scale.x > 1.f || transform.scale.x < 1.f ||
                    transform.scale.y > 1.f || transform.scale.y < 1.f)
                {
                    model = glm::translate(model, glm::vec3{transform.position, 0.f});
                    model = glm::translate(model, glm::vec3{(sprite.width * transform.scale.x) * 0.5f, (sprite.height * transform.scale.y) * 0.5f, 0.f});
                    model = glm::rotate(model, glm::radians(transform.rotation), glm::vec3{0.f, 0.f, 1.f});
                    model = glm::translate(model, glm::vec3{(sprite.width * transform.scale.x) * -0.5f, (sprite.height * transform.scale.y) * -0.5f, 0.f});
                    
                    model = glm::scale(model, glm::vec3{transform.scale, 1.f});

                    model = glm::translate(model, glm::vec3{-transform.position, 0.f});
                } */

                m_pBatchRenderer->AddSprite(spriteRect, uvRect, pTexture->GetID(), sprite.layer, model, sprite.color);
            }

            m_pBatchRenderer->End();
            m_pBatchRenderer->Render();
            spriteShader->Disable();
        }

}