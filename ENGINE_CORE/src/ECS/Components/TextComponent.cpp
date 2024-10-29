#include "Core/ECS/Components/TextComponent.hpp"
#include <entt/entt.hpp>


void ENGINE_CORE::ECS::TextComponent::CreateTextLuaBinding(sol::state& lua)
{
    lua.new_usertype<TextComponent>(
        "TextComponent",
        "type_id", entt::type_hash<TextComponent>::value,
        sol::call_constructor,
        sol::factories(
            [](const std::string& sFontName, const std::string& sTextStr, ENGINE_RENDERING::Color color, int padding, float wrap) {
                return TextComponent{
                    .sFontName = sFontName,
                    .sTextStr = sTextStr,
                    .padding = padding,
                    .wrap = wrap,
                    .color = color
                };
            }
        ),
        "textStr", &TextComponent::sTextStr,
        "fontName", &TextComponent::sFontName,
        "padding", &TextComponent::padding,
        "bHidden", &TextComponent::bHidden,
        "wrap", &TextComponent::wrap,
        "color", &TextComponent::color
    );
}