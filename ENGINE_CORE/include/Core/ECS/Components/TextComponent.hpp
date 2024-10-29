#pragma once

#include <sol/sol.hpp>
#include <Rendering/Essentials/Vertex.hpp>


namespace ENGINE_CORE::ECS
{

    struct TextComponent
    {
        std::string sFontName{"pixel"}, sTextStr{""};
        int padding;
        float wrap{-1.f};
        ENGINE_RENDERING::Color color{255, 255, 255, 255};
        bool bHidden{false};

        static void CreateTextLuaBinding(sol::state& lua);
    };

}