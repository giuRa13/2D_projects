#pragma once 

#include "Vertex.hpp"


namespace ENGINE_RENDERING
{

    struct Batch
    {
        GLuint numIndices{0}, offset{0}, textureID{0};
    };

    struct SpriteGlyph
    {
        Vertex topLeft, bottomLeft, topRight, bottomRight;
        int layer;
        GLuint textureID;
    };

}