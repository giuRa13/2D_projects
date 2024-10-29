#include "Core/Systems/RenderUISystem.hpp"
#include "Core/ECS/Components/TextComponent.hpp"
#include "Core/ECS/Components/TransformComponent.hpp"
#include <Rendering/Essentials/Font.hpp>
#include "Core/Resources/AssetManager.hpp"
#include <Logger/Logger.hpp>

using namespace ENGINE_CORE::ECS;
using namespace ENGINE_RESOURCES;


namespace ENGINE_CORE::Systems
{

    RenderUISystem::RenderUISystem(ENGINE_CORE::ECS::Registry& registry)
        : m_Registry{registry}
        , m_pSpriteRenderer{std::make_unique<ENGINE_RENDERING::SpriteBatchRenderer>()}
        , m_pTextRenderer{std::make_unique<ENGINE_RENDERING::TextBatchRenderer>()}
        , m_pCamera2D{ nullptr}
    {
        m_pCamera2D = std::make_unique<ENGINE_RENDERING::Camera2D>(640, 480);
        m_pCamera2D->Update();
    }


    void RenderUISystem::Update(entt::registry& registry)
    {
        auto& assetManager = m_Registry.GetContext<std::shared_ptr<AssetManager>>();
        auto pFontShader = assetManager->GetShader("font");

        if(!pFontShader)
        {
            ENGINE_ERROR("Failed to get the Font Shader from the Asset Manager");
            return;
        }

        auto cam_mat = m_pCamera2D->GetCameraMatrix();
        auto textView = registry.view<TextComponent, TransformComponent>();

        pFontShader->Enable();
        pFontShader->SetUniformMat4("uProjection", cam_mat);

        m_pTextRenderer->Begin();

        for(auto entity : textView)
        {
            const auto& text = textView.get<TextComponent>(entity);
            
            if(text.sFontName.empty() || text.bHidden)
                continue;

            const auto& pFont = assetManager->GetFont(text.sFontName);
            
            if(!pFont)
            {
                ENGINE_ERROR("Font [{}] does not exsists in Asset Manager", text.sFontName);
                continue;
            }

            const auto& transform = textView.get<TransformComponent>(entity);

            glm::mat4 model{1.f};

            if  ( transform.rotation > 0.f || transform.rotation < 0.f || 
                  transform.scale.x > 1.f || transform.scale.x < 1.f ||
                  transform.scale.y > 1.f || transform.scale.y < 1.f)
            {
                model = glm::translate(model, glm::vec3{transform.position, 0.f});
                model = glm::translate(model, glm::vec3{(pFont->GetFontSize() * transform.scale.x) * 0.5f, (pFont->GetFontSize() * transform.scale.y) * 0.5f, 0.f});
                model = glm::rotate(model, glm::radians(transform.rotation), glm::vec3{0.f, 0.f, 1.f});
                model = glm::translate(model, glm::vec3{(pFont->GetFontSize() * transform.scale.x) * -0.5f, (pFont->GetFontSize() * transform.scale.y) * -0.5f, 0.f});
                    
                model = glm::scale(model, glm::vec3{transform.scale, 1.f});

                model = glm::translate(model, glm::vec3{-transform.position, 0.f});
            } 

            m_pTextRenderer->AddText(text.sTextStr, pFont, transform.position, text.padding, text.wrap, text.color, model);
        }

        m_pTextRenderer->End();
        m_pTextRenderer->Render();
        pFontShader->Disable();
    }

}