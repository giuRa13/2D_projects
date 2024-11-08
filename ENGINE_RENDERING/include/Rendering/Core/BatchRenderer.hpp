#pragma once

#include "Rendering/Core/Batcher.hpp"
#include "Rendering/Essentials/BatchTypes.hpp"


namespace ENGINE_RENDERING
{

    class SpriteBatchRenderer : public Batcher<Batch, SpriteGlyph>
    {

    private:
        void Initialize();
        virtual void GenerateBatches() override;

    
    public:
        SpriteBatchRenderer();
        virtual ~SpriteBatchRenderer() = default;

        virtual void End() override;
        virtual void Render() override;

        void AddSprite(
            const glm::vec4& spriteRect, 
            const glm::vec4 uvRect, 
            GLuint textureID, 
            int layer = 0, 
            glm::mat4 model = glm::mat4{1.f},
            const Color& color = Color{.r = 255, .g = 255, .b = 255, .a = 255});

    };

}