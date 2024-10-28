#pragma once

#include "Vertex.hpp"
#include "Font.hpp"
#include <string>
#include <memory>


namespace ENGINE_RENDERING
{

    struct Line
    {
        glm::vec2 p1{0.f}, p2{0.f};
        float lineWidth{1.f};
        Color color{};
    };

    struct Rect
    {
        glm::vec2 position{0.f};
        float width{0.f}, height{0};
        Color color{};
    };

    struct Circle
    {
        glm::vec2 position{0.f}, p2{0.f};
        float lineThickness{1.f}, radius{0.f};
        Color color{};
    };

    struct Text
    {
        glm::vec2 position{0.f};
        std::string textStr{""};
        float wrap{-1.f};
        std::shared_ptr<Font> pfont{nullptr};
        Color color{255, 255, 255, 255};
    };

}