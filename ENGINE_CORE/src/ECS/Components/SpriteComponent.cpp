#include "Core/ECS/Components/SpriteComponent.hpp"
#include "Core/Resources/AssetManager.hpp"
#include <Logger/Logger.hpp>

using namespace ENGINE_RESOURCES;


void ENGINE_CORE::ECS::SpriteComponent::CreateSpriteLuaBind(sol::state& lua, ENGINE_CORE::ECS::Registry& registry)
{

    lua.new_usertype<ENGINE_RENDERING::Color>(
        "Color",
        sol::call_constructor,
        sol::factories(
            [](GLubyte r, GLubyte g, GLubyte b, GLubyte a) {
                return ENGINE_RENDERING::Color{ .r = r, .g = g, .b = b, .a = a};
            }
        ),
        "r", &ENGINE_RENDERING::Color::r,
        "g", &ENGINE_RENDERING::Color::g,
        "b", &ENGINE_RENDERING::Color::b,
        "a", &ENGINE_RENDERING::Color::a
    );

    lua.new_usertype<UVs>(
        "UVs",
        sol::call_constructor,
        sol::factories(
            [](float u, float v) { return UVs{.u = u, .v = v}; }
        ),
        "u", &UVs::u,
        "v", &UVs::v,
        "uv_width", &UVs::uv_width,
        "uv_height", &UVs::uv_height
    );

    lua.new_usertype<SpriteComponent>(
        "Sprite",
        "type_id", &entt::type_hash<SpriteComponent>::value,
        sol::call_constructor,
        sol::factories(
            [](const std::string& textureName, float width, float height, int start_x, int start_y, int layer) {
                return SpriteComponent{
                    .width = width,
                    .height = height,
                    .uvs = UVs{},
                    .color = ENGINE_RENDERING::Color{255, 255, 255, 255},
                    .start_x = start_x,
                    .start_y = start_y,
                    .layer = layer,
                    .texture_name = textureName
                };
            }
        ),
        "texture_name", &SpriteComponent::texture_name,
        "width", &SpriteComponent::width,
        "height", &SpriteComponent::height,
        "start_x", &SpriteComponent::start_x,
        "start_y", &SpriteComponent::start_y,
        "layer", &SpriteComponent::layer,
        "bHidden", &SpriteComponent::bHidden,
        "uvs", &SpriteComponent::uvs,
        "color", &SpriteComponent::color,
        "generate_uvs", [&](SpriteComponent& sprite) {
            auto& assetManager = registry.GetContext<std::shared_ptr<AssetManager>>();
            auto& texture = assetManager->GetTexture(sprite.texture_name);

            if(texture.GetID() == 0)
            {
                ENGINE_ERROR("Failet to generate uvs -- Texture [{}] -- Does not exists or invalid", sprite.texture_name);
                return;
            }

            sprite.generate_uvs(texture.GetWidth(), texture.GetHeight());
        }
    );
}