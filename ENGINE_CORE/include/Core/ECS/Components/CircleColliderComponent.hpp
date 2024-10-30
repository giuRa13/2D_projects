#pragma once

#include <glm/glm.hpp>
#include <sol/sol.hpp>


namespace ENGINE_CORE::ECS
{

    struct CircleColliderComponent
    {
        float radius{0.f};

        glm::vec2 offset{glm::vec2{0}};

        bool bColliding{false};

        [[nodiscard]] std::string to_string() const;

        static void CreateLuaCircleColliderBind(sol::state& lua);
    };

}